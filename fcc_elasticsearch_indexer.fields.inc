<?php

/**
 * @file fcc_elasticsearch_indexer.fields.inc
 * Indexing functions for node field
 */
/* ------------------------------------------------------------------------------
  # Article Fields
  ------------------------------------------------------------------------------ */

/**
 * Retrieve the body fields.
 *
 * @param $node entity
 * @return array
 */
function fcc_es_get_body_fields($node) {
    $body = $node->body[LANGUAGE_NONE][0];

    $body_fields = array();
    $body_fields['value'] = $body['value'];
    $body_fields['summary'] = $body['summary'];
    $body_fields['safe_value'] = strip_tags($body['safe_value']);
    $body_fields['safe_summary'] = strip_tags($body['safe_summary']);

    unset($body); // unset parameters
    unset($node); // unset parameters

    return $body_fields;
}

/**
 * Retrieve the domains field.
 *
 * @param $node entity
 * @return array
 */
function fcc_es_get_domains_field($node) {
    $domains_field = ($node->domains) ? array_values($node->domains) : NULL;

    unset($node); // unset parameters

    return $domains_field;
}

/**
 * Retrieve the published date field.
 *
 * @param $node entity
 * @return array
 */
function fcc_es_get_published_date($node) {
    if ($node->workbench_moderation['published']->timestamp) {
        $published_date = $node->workbench_moderation['published']->timestamp;
        unset($node->workbench_moderation['published']->timestamp); // unset node value
    }
    else {
        $published_date = $node->created;
    }
    unset($node); // unset parameters

    return $published_date;
}

/**
 * Prepare date terms to send to ES.
 *
 * @param string $timestamp
 *
 * @return array
 */
function fcc_es_get_date_terms($timestamp) {
    $date_terms = array(
      'year' => (int) date('Y', $timestamp),
      'month' => (int) date('m', $timestamp),
      'week' => (int) date('W', $timestamp),
      'dayofyear' => (int) date('z', $timestamp),
      'day' => (int) date('d', $timestamp),
      'dayofweek' => (int) date('w', $timestamp),
      'dayofweek_iso' => (int) date('N', $timestamp),
      'hour' => (int) date('H', $timestamp),
      'minute' => (int) date('i', $timestamp),
      'second' => (int) date('s', $timestamp),
      'm' => (int) (date('Y', $timestamp) . date('m', $timestamp) ), // yearmonth
    );
    unset($timestamp); // unset parameters

    return $date_terms;
}

/**
 * Helper function to sanitize and build domain specific urls.
 *
 * @param $node entity
 * @return string
 */
function fcc_es_get_url($node) {
    if (!empty($node->field_override_url)) {
        $url = $node->field_override_url[$node->language][0]['value']; // URL Override
    }
    else {
        $url = fcc_es_get_parent_domain($node) . '/node/' . $node->nid; // No URL Override
    }
    unset($node); // unset parameters

    return $url;
}

/**
 * Returns parent domain from newspaper name
 *
 * @param $node entity
 * @return string
 */
function fcc_es_get_parent_domain($node) {
    $newspaper = fcc_es_get_data_field_value('field_newspaper', $node);
    $subdomain = fcc_es_newspaper_to_subdomain($newspaper);

    unset($newspaper); // unset newspaper
    unset($node); // unset parameter

    return $subdomain;
}

/**
 * Retrieve the author fields.
 *
 * @param $node entity
 * @return array
 */
function fcc_es_get_author_fields($node) {
    $author = NULL;
    if ($node->uid >= 1) {
        //$user = user_load($node->uid);
        $user = reset(_fcc_es_entity_load('user', array($node->uid)));
        //reset($user);
        $author['realname'] = fcc_es_get_author_name($user, $node);
        $author['first_name'] = $user->field_first_name[LANGUAGE_NONE][0]['value'];
        $author['last_name'] = $user->field_last_name[LANGUAGE_NONE][0]['value'];
    }
    unset($node); // unset parameters
    unset($user); // unset user

    return $author;
}

/**
 * Retrieve the author real name.
 *
 * @param Object $author The Author object on which the operation is being performed.
 * @return string The name of the author.
 */
function fcc_es_get_author_name($author, $node) {
    if (isset($author->realname) && ( '' !== $author->realname )) :
        $author_name = $author->realname;
    elseif (( isset($author->field_first_name[$node->language][0]['value']) && $author->field_first_name[$node->language][0]['value'] ) || ( isset($author->field_last_name[$node->language][0]['value']) && $author->field_last_name[$node->language][0]['value'] )) :
        $author_name = $author->field_first_name[$node->language][0]['value'] . ' ' . $author->field_last_name[$node->language][0]['value'];
    else :
        if (isset($author->name)) :
            $author_name = $author->name;
        else :
            $author_name = '';
        endif;
    endif;

    unset($author); // Unset parameter
    unset($node); // Unset parameter

    return $author_name;
}

/**
 * Create the featured image url.
 * @param type $uri
 * @return string
 */
function fcc_es_get_image_url($uri) {
    $url = preg_replace('(.+(?=sites\/default\/files\/))', '', $uri);
    //$url = preg_replace( '(https?:\/\/)', '', $uri );
    //$url = preg_replace( '(^default)', '', $url );
    //$url = preg_replace( '(\?q=)', '', $url );
    $url = preg_replace('(\itok=.+)', '', $url); // token removal
    $url = preg_replace('(\?$)', '', $url); // token removal
    $url = preg_replace('(\&$)', '', $url); // token removal
    $url = 'www.fccnn.com/' . $url; // TODO: find better or less static method?
    return $url;
}

/**
 * Retrieve the featured image.
 *
 * drush php-eval 'print_r( fcc_es_get_featured_image( node_load(11) ) );'
 * @param $node entity
 * @return array
 */
function fcc_es_get_featured_image($node) {
    $image_field = $node->field_image[LANGUAGE_NONE][0];
    if (!empty($image_field)) {
        $image_field['full'] = fcc_es_get_image_url(file_create_url($image_field['uri']));
        $image_field['full_1000'] = fcc_es_get_image_url(image_style_url('full_1000', $image_field['uri']));
        $image_field['16x9_860'] = fcc_es_get_image_url(image_style_url('16x9_860', $image_field['uri']));
        $image_field['16x9_430'] = fcc_es_get_image_url(image_style_url('16x9_430', $image_field['uri']));
        $image_field['square_300'] = fcc_es_get_image_url(image_style_url('square_300', $image_field['uri']));
    }
    unset($node); // unset parameter
    unset($image_field['rdf_mapping']);

    return $image_field;
}

/**
 * Retrieve the search thumbnail image.
 *
 * drush php-eval 'print_r( fcc_es_get_featured_image( node_load(11) ) );'
 * @param $node entity
 * @return array
 */
function fcc_es_get_search_thumbnail($node) {
    if (empty($node->field_image)) {
        return; // no image on node
    }

    $uri = $node->field_image[LANGUAGE_NONE][0]['uri'];
    $fid = $node->field_image[LANGUAGE_NONE][0]['fid'];

    if (!empty($uri)) {
        $search_thumb = fcc_es_get_image_url(image_style_url('square_300', $uri));
    }
    else {
        $search_thumb = fcc_es_get_image_url(image_style_url('square_300', file_load($fid)->uri));
    }

    // memory managment
    unset($node);
    unset($uri);
    unset($fid);

    return $search_thumb;
}

/**
 * Retrieve the search thumbnail file name.
 *
 * drush php-eval 'print_r( fcc_es_get_thumbnail_filename( node_load(11) ) );'
 * @param $node entity
 * @return string
 */
function fcc_es_get_thumbnail_filename($node) {
    if (empty($node->field_image)) {
        return; // no image on node
    }

    $filename = $node->field_image[LANGUAGE_NONE][0]['filename'];
    if (empty($filename)) {
        $fid = $node->field_image[LANGUAGE_NONE][0]['fid'];
        $filename = file_load($fid)->filename;
    }

    // memory managment
    unset($node);
    unset($fid);

    return $filename;
}

/**
 * Retrieve the images fields.
 *
 * drush php-eval 'print_r( fcc_es_get_image_fields( node_load(11) ) );'
 * @param $node entity
 * @return array
 */
function fcc_es_get_image_fields($node) {
    $field_image = $node->field_image[LANGUAGE_NONE];

    $images_array = array();
    if (!empty($field_image)) {
        foreach ($field_image as $image) {

            $image_field = $image;
            unset($image_field['rdf_mapping']);

            if (!empty($image_field['uri'])) {
                $uri = $image_field['uri'];
            }
            else {
                $uri = file_load($image_field['fid'])->uri;
            }

            $image_field['full'] = fcc_es_get_image_url(file_create_url($uri));
            $image_field['full_1000'] = fcc_es_get_image_url(image_style_url('full_1000', $uri));
            $image_field['16x9_860'] = fcc_es_get_image_url(image_style_url('16x9_860', $uri));
            $image_field['16x9_430'] = fcc_es_get_image_url(image_style_url('16x9_430', $uri));
            $image_field['square_200'] = fcc_es_get_image_url(image_style_url('square_200', $uri));
            $images_array[] = $image_field;
        }
    }
    unset($node); // unset parameter
    unset($field_image); // unset image
    unset($image_field); // unset image
    unset($uri); // unset uri

    return $images_array;
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
    $terms = NULL;
    $entity_field = $entity->{$field}[LANGUAGE_NONE];
    if (is_array($entity_field) || $entity_field instanceof Traversable) {
        // Create array of taxonomy ids to load
        foreach ($entity_field as $field => $value) {
            $tids[] = $value['tid'];
        }
        // Load taxonomy term entities
        if (!empty($tids)) {
            $term = _fcc_es_entity_load('taxonomy_term', $tids);
            //$term = taxonomy_term_load($tid['tid']);
        }
        // Create taxonomy terms array
        foreach ($tids as $tid) {
            if (!empty($term)) {
                $terms[] = array(
                  'tid' => $term[$tid]->tid,
                  'name' => $term[$tid]->name,
                );
            }
        }
        unset($tids); // Unset taxonomy ids
        unset($term); //Unset term load
    }

    unset($entity_field); // Unset entity field

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
function fcc_es_get_file_fields($field, $entity) {
    $videos = NULL;
    $entity_field = $entity->{$field}[LANGUAGE_NONE];
    if (!$entity_field) {
        return NULL;
    }
    else {
        if (is_array($entity_field) || $entity_field instanceof Traversable) {
            $fids = array_column($entity->{$field}[LANGUAGE_NONE], 'fid');
            $files = _fcc_es_entity_load('file', $fids);
            //$files = file_load_multiple($fids);
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

    unset($field); // Unset parameter
    unset($entity); // Unset parameter
    unset($entity_field); // Unset entity field
}

/**
 * Gets media stream paths
 *
 * @param type $file
 * @return type
 */
function fcc_es_get_media_stream_paths($file) {
    $stream = NULL;
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

    unset($file); // unset parameter

    return $stream;
}

/**
 * Fetch poll fields from an entity.
 *
 * @param type $entity
 * @return type
 */
function fcc_es_get_poll_fields($entity) {
    $poll = array();
    $poll_fields = array('field_poll_id', 'field_poll_title');

    foreach ($poll_fields as $field) {
        $entity_field = $entity->{$field}[LANGUAGE_NONE];
        if (is_array($entity_field) || $entity_field instanceof Traversable) {
            $value = array_shift(array_column($entity_field, 'value'));
        }
        else {
            return null;
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
        unset($entity_field); // unset entity field
    }
    unset($entity); // unset parameter
    unset($poll_fields); // unset poll fields

    return $poll;
}

/* ------------------------------------------------------------------------------
  # Obituary Fields
  ----------------------------------------------------------------------------- */

/**
 * Retrieve obit fields from obituaries node enitity.
 *
 * @param Object $entity
 * @return array An array of obit fields.
 */
function fcc_es_get_obit_fields($entity) {
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

/* ------------------------------------------------------------------------------
  # Letters Fields
  ----------------------------------------------------------------------------- */

/**
 * Retrieve letter fields from letters node enitity.
 *
 * @param type $entity
 * @return type
 */
function fcc_es_get_letter_fields($entity) {
    $letter = array();

    // Publisher
    $publisher = fcc_es_get_author_fields($entity);
    $letter['publisher']['info'] = $publisher;
    $letter['publisher']['name'] = $publisher['name'];
    unset($publisher); // unset publisher
    // Author
    $letter['author']['name'] = fcc_es_get_data_field_value('field_author', $entity);
    $letter['author']['phone_number'] = fcc_es_get_data_field_value('field_phone_number', $entity);
    $letter['author']['city'] = fcc_es_get_data_field_value('field_city', $entity);

    return $letter;
}

/* ------------------------------------------------------------------------------
  # Classifies Fields // TODO: Remove if unnecessary
  ----------------------------------------------------------------------------- */

/**
 * Retrieve classified fields from classified node enitity.
 *
 * @param type $entity
 * @return type
 */
function fcc_es_get_classified_fields($entity) {
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

/* ------------------------------------------------------------------------------
  # General
  ----------------------------------------------------------------------------- */

/**
 * Strips a single input value from data field.
 *
 * @param type $field
 * @param type $entity
 * @return type
 */
function fcc_es_get_data_field_value($field, $entity, $input = 'value') {
    $value = NULL;
    if (!empty($entity->{$field})) {
        $value = $entity->{$field}[LANGUAGE_NONE][0][$input];
    }
    return $value;
}

/**
 * Returns subdomain address based on parent newspaper
 *
 * @param string $newspaper
 * @return type
 */
function fcc_es_newspaper_to_subdomain($newspaper) {

    $site_name = ($newspaper) ? $newspaper : 'FCCNN';
    $domain_id = _es_get_sitename_domain_id($site_name);
    $subdomain = _es_get_subdomain($domain_id);

    //Unset variables
    unset($domain_id); // unset domain id
    unset($site_name); // unset site name

    return $subdomain;
}
