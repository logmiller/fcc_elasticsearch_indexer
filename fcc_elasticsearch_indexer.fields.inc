<?php

/**
 * @file fcc_elasticsearch_indexer.fields.inc
 * Indexing functions for node field
 */

/*------------------------------------------------------------------------------
# Article Fields
------------------------------------------------------------------------------*/

/**
 * Retrieve the author fields.
 *
 * @param type $nid
 * @param type $uid
 */
function fcc_es_get_author_fields( $nid, $uid ) {
	$author = array();
	if ( $uid > 1 ) {
		$user = user_load( $uid );
		// Author name
		$author['realname'] = fcc_es_get_author_name( $user, $nid );
		$author['name'] = $user->name;
		$author['url'] = url( drupal_get_path_alias( 'user/$uid' ) );
		if ( ! empty( $user->picture ) ) {
			$author['picture'] = array(
				'uri' => $user->picture->uri,
				'width' => $user->picture->image_dimensions['width'],
				'height' => $user->picture->image_dimensions['height'],
			);
		}
		$author['byline'] = "By <a href='{$author['url']}'>{$author['name']}</a>";
		$author['bio'] = ( ! empty( $user->field_bio ) ) ? $user->field_bio[ LANGUAGE_NONE ][0]['value'] : '';
		$author['email'] = ( ! empty( $user->mail ) ) ? $user->mail : '';
		$author['phone'] = ( ! empty( $user->field_phone_number ) ) ? $user->field_phone_number[ LANGUAGE_NONE ][0]['value'] : '';
		$author['twitter'] = ( ! empty( $user->twitter ) ) ? $user->field_twitter[ LANGUAGE_NONE ][0]['value'] : '';
		$author['google'] = ( ! empty( $user->field_google_plus ) ) ? $user->field_google_plus[ LANGUAGE_NONE ][0]['value'] : '';
	}
	return $author;
}

/**
 * Retrieve the author real name.
 *
 * @param Object $author The Author object on which the operation is being performed.
 * @return string The name of the author.
 */
function fcc_es_get_author_name( $author, $node ) {
	if ( isset( $author->realname ) && (  '' !== $author->realname ) ) :
		$author_name = $author->realname;
		elseif ( ( isset( $author->field_first_name[ $node->language ][0]['value']) && $author->field_first_name[ $node->language ][0]['value'] ) || ( isset( $author->field_last_name[ $node->language ][0]['value'] ) && $author->field_last_name[ $node->language ][0]['value'] ) ) :
			$author_name = $author->field_first_name[ $node->language ][0]['value'] . ' ' . $author->field_last_name[ $node->language ][0]['value'];
	else :
		if ( isset( $author->name ) ) :
			$author_name = $author->name;
		else :
			$author_name = '';
		endif;
	endif;
	return $author_name;
}

/**
 * Fetch all taxonomy terms from an entity.
 *
 * @param type $field
 * @param Object $entity
 *
 * @return array
 *   Array with tids of entity.
 */
function fcc_es_get_taxonomy_terms($field, $entity) {
   $terms = array();
   $entity_field = $entity->{$field}[LANGUAGE_NONE];
   if (is_array($entity_field) || $entity_field instanceof Traversable) {
       foreach ($entity_field as $tid) {
           $term = taxonomy_term_load($tid['tid']);
           if (!empty($term->tid)) {
               $terms[$term->vocabulary_machine_name][] = array(
                 'tid' => $term->tid,
                 'name' => $term->name,
               );
           }
       }
       unset($entity_field);
   }
   return $terms;
}

/**
 * Retrieve file fields
 *
 * @param type $field
 * @param Object $entity
 *
 * @return array
 *   Array with tids of entity.
 */
function fcc_es_get_file_fields( $field, $entity ) {
   $videos = array();
   $entity_field = $entity->{$field}[LANGUAGE_NONE];
   if (is_array($entity_field) || $entity_field instanceof Traversable) {
       $fids = array_column($entity->{$field}[LANGUAGE_NONE], 'fid');
       $files = file_load_multiple($fids);
       unset($fids); // unset file ids
       foreach ($files as $file) {
           $media = fcc_es_get_media_stream_paths($file);
           $video = array(
             'type' => $file->type,
             'fid' => $file->fid,
             'uid' => $file->uid,
             'filename' => $file->filename,
             'uri' => $file->uri,
             'filemime' => $file->filemime,
           );

           $videos[] = array_merge($video, $media);
           unset($media); // unset media
       }
       unset($file); // unset file
   }
   return $videos;
}

/**
 * Gets media stream paths
 *
 * @param type $file
 * @return type
 */
function fcc_es_get_media_stream_paths( $file ) {
   $stream = array();
   switch ($file->filemime) :
       case 'video/jwplayer':
           $key = substr($file->uri, 13, -9);
           $player = substr($file->uri, 22);
           $stream = array(
             'uri_media' => 'content.jwplatform.com/videos/' . $key . '.mp4',
             'key' => $key,
             'player' => $player,
           );
           break;
       default :
           $stream = array(
             'uri_media' => file_create_url($file->uri),
           );
           break;
   endswitch;
   return $stream;
}

/**
 * Fetch poll fields from an entity.
 *
 * @param type $entity
 * @return type
 */
function fcc_es_get_poll_fields( $entity ) {
   $poll = array();
   $poll_fields = array('field_poll_id', 'field_poll_title');

   foreach ($poll_fields as $field) {
       $entity_field = $entity->{$field}[LANGUAGE_NONE];
       if (is_array($entity_field) || $entity_field instanceof Traversable) {
           $value = array_shift(array_column($entity_field, 'value'));
       }
       else {
           return $poll;
       }
       if ($field == 'field_poll_id') {
           $poll = array(
             'id' => $value,
             'url' => 'polldaddy.com/poll/' . $value,
           );
       }
       elseif ($field == 'field_poll_title') {
           $poll['title'] = $value;
       }
       unset($value); // unset poll value
   }
   unset($poll_fields); // unset poll fields
   return $poll;
}

/*------------------------------------------------------------------------------
 # Obituary Fields
 -----------------------------------------------------------------------------*/

/**
 * Retrieve obit fields from obituaries node enitity.
 *
 * @param Object $entity
 * @return array An array of obit fields.
 */
function fcc_es_get_obit_fields( $entity ) {
   $obit = array();

   $birth = fcc_es_get_data_field_value('field_lifespan', $entity);
   $death = fcc_es_get_data_field_value('field_lifespan', $entity, 'value2');
   // Lifespan: Birth
   if ($birth != $death) {
       $obit['lifespan']['birth']['value'] = $birth;
       $obit['lifespan']['birth']['formatted_value'] = date('l, F j, Y', strtotime($birth));
   }
   unset($birth); // Unset birth variable
   // Lifespan: Death
   if (strpos($death, '00:00:00')) {
       $obit['lifespan']['death']['value'] = $death;
       $obit['lifespan']['death']['formatted_value'] = date('l, F j, Y', strtotime($death));
   }
   unset($death); // Unset death variable
   // Funeral Home
   $obit['funeral_home']['name'] = fcc_es_get_data_field_value('field_funeral_home', $entity);
   $obit['funeral_home']['url'] = fcc_es_get_data_field_value('field_funeral_home_url', $entity);

   // Logo
   $obit['logo'] = $entity->field_logo[LANGUAGE_NONE];

   // Location
   $obit['location']['city'] = fcc_es_get_data_field_value('field_obit_city', $entity);
   $obit['location']['state'] = fcc_es_get_data_field_value('field_obit_state', $entity);
   return $obit;
}

/*------------------------------------------------------------------------------
 # Letters Fields
 -----------------------------------------------------------------------------*/

/**
 * Retrieve letter fields from letters node enitity.
 *
 * @param type $entity
 * @return type
 */
function fcc_es_get_letter_fields( $entity ) {
   $letter = array();

   // Publisher
   $publisher = fcc_es_get_author_fields($entity->nid, $entity->uid);
   $letter['publisher']['info'] = $publisher;
   $letter['publisher']['name'] = $publisher['name'];
   unset($publisher); // unset publisher
   // Author
   $letter['author']['name'] = fcc_es_get_data_field_value('field_author', $entity);
   $letter['author']['phone_number'] = fcc_es_get_data_field_value('field_phone_number', $entity);
   $letter['author']['city'] = fcc_es_get_data_field_value('field_city', $entity);

   return $letter;
}


/*------------------------------------------------------------------------------
 # Classifies Fields // TODO: Remove if unnecessary
 -----------------------------------------------------------------------------*/

/**
 * Retrieve classified fields from classified node enitity.
 *
 * @param type $entity
 * @return type
 */
function fcc_es_get_classified_fields( $entity ) {
   $classified = array();

   $classified['classification'] = fcc_es_get_taxonomy_terms('field_classification', $entity);
   $classified['creator_domain'] = fcc_es_get_data_field_value('field_creator_domain', $entity);
   $classified['featured'] = fcc_es_get_data_field_value('field_featured', $entity);
   $classified['image_url'] = fcc_es_get_data_field_value('field_image_url', $entity);
   //$classified['field_name'] =$entity->field_name; //This is empty, yo.
   $classified['posted'] = fcc_es_get_data_field_value('field_posted', $entity);
   $classified['published_to'] = fcc_es_get_data_field_value('field_published_to', $entity);
   $classified['type'] = fcc_es_get_data_field_value('field_type', $entity);

   return $classified;
}

/*------------------------------------------------------------------------------
 # General
 -----------------------------------------------------------------------------*/

/**
 * Strips a single input value from data field.
 *
 * @param type $field
 * @param type $entity
 * @return type
 */
function fcc_es_get_data_field_value( $field, $entity, $input = 'value' ) {
	$entity_field = $entity->{$field}[ LANGUAGE_NONE ];
	if ( is_array( $entity_field ) || $entity_field instanceof Traversable ) {
		$value = array_shift( array_column( $entity_field, $input ) );
	}
	unset( $entity_field ); //unset entity field
	return $value;
}
