<?php

/**
 * Converts node entity type to document.
 *
 * @param type $index
 * @param type $offset
 * @param type $batch_size
 * @return array
 */
function _fcc_elasticsearch_indexer_node_article_to_documents($node_type, $offset, $batch_size) {
    // Load entities
    $entities = _fcc_es_process_node_entities($node_type, $offset, $batch_size);

    // Build document array
    // Notice: Command and Content arrays must be separated for bulk indexing to work.
    $document = array();
    foreach ($entities as $entity) {
			$document['body'][] = [ 'index' => _fcc_elasticsearch_indexer_prepare_bulk_document_metadata( $entity ) ];
                        $document['body'][] = call_user_func('_fcc_elasticsearch_indexer_prepare_' . $node_type, $entity); // Body Fields
    }
    unset($entities); // unset entities
    return $document;
}

/**
 * Converts node entity obits to document.
 *
 * @param type $index
 * @param type $offset
 * @param type $batch_size
 * @return array
 */
function _fcc_elasticsearch_indexer_node_obituaries_to_documents($index, $offset, $batch_size) {
    // Load entities
    $entities = _fcc_es_process_node_entities('obituaries', $offset, $batch_size);

    // Build document array
    // Notice: Command and Content arrays must be separated for bulk indexing to work.
    $document = array();
    foreach ($entities as $entity) {

        // Notice: No URL Override Field
        $url = url('node/' . $entity->nid);

        //Field Author
        // Notice: Not Sure if Author is Required for Obits
        $author = fcc_es_get_author_fields( $entity );

        //Field Obits
        $obit = fcc_es_get_obit_fields($entity);

        //Field Image
        $image_field = $entity->field_image[LANGUAGE_NONE];
        if (!empty($image_field)) {
            $images = array_map(function( $img ) {
                $img_alt = $img['alt'];
                $img_description = $img['description'];
                $img = file_load($img['fid']);
                $img->url = (!empty($image_field[0]['uri']) ) ? file_create_url($img_uri[0]['uri']) : file_create_url($img->uri);
                $img->full_1000 = (!empty($image_field[0]['uri']) ) ? image_style_url('full_1000', $image_field[0]['uri']) : file_create_url($img->uri);
                $img->featured_860 = (!empty($image_field[0]['uri']) ) ? image_style_url('16x9_860', $image_field[0]['uri']) : file_create_url($img->uri);
                $img->featured_620 = (!empty($image_field[0]['uri']) ) ? image_style_url('16x9_620', $image_field[0]['uri']) : file_create_url($img->uri);
                $img->featured_430 = (!empty($image_field[0]['uri']) ) ? image_style_url('16x9_430', $image_field[0]['uri']) : file_create_url($img->uri);
                $img->featured_315 = (!empty($image_field[0]['uri']) ) ? image_style_url('16x9_315', $image_field[0]['uri']) : file_create_url($img->uri);
                $img->square_300 = (!empty($image_field[0]['uri']) ) ? image_style_url('square_300', $image_field[0]['uri']) : file_create_url($img->uri);
                $img->medium = (!empty($image_field[0]['uri']) ) ? image_style_url('medium', $image_field[0]['uri']) : file_create_url($img->uri);
                $img->image_caption = htmlentities($img_alt, ENT_QUOTES, 'UTF-8');
                $img->image_description = $img_description;
                return $img;
            }, $image_field);
        }

        //Document: Command
        $document['body'][] = array(
          'index' => array(
            '_index' => $index, // TODO make dynamic for property name
            '_type' => $entity->type,
            '_id' => $entity->nid,
          ),
        );
        //Document: Content
        $document['body'][] = array(
          // Default
          'nid' => $entity->nid,
          'title' => $entity->title,
          'uid' => $entity->uid,
          'name' => $author['name'], // Obits has Author?
          'picture' => $entity->picture,
          'created' => $entity->created,
          'changed' => $entity->changed,
          'revision_timestamp' => $entity->revision_timestamp,
          'vid' => $entity->vid,
          'revision_uid' => $entity->revision_uid,
          'type' => $entity->type,
          'status' => $entity->status,
          'promote' => $entity->promote,
          'sticky' => $entity->sticky,
          'url' => $_SERVER['HTTP_HOST'] . $url, //TODO
          'author' => $author, // Obits has Author?
          'path' => $entity->path,
          'domains' => array_values($entity->domains),
          'subdomains' => $entity->subdomains,
          'domain_site' => $entity->domain_site,
          //Custom Fields
          'body' => $entity->body[LANGUAGE_NONE][0],
          'field_image' => $images,
          'funeral_home' => $obit['funeral_home'],
          'lifespan' => $obit['lifespan'],
          'logo' => $obit['logo'], // TODO: Image type: Handle Like field_image?
          'location' => $obit['location'],
        );
        unset($obit); // Unset Obit array
    }
    unset($entities); // unset entities

    return $document;
}

/**
 * Converts node entity letters to document.
 *
 * @param type $index
 * @param type $offset
 * @param type $batch_size
 * @return array
 */
function _fcc_elasticsearch_indexer_node_letters_to_documents($index, $offset, $batch_size) {
    // Load entities
    $entities = _fcc_es_process_node_entities('letters', $offset, $batch_size);

    // Build document array
    // Notice: Command and Content arrays must be separated for bulk indexing to work.
    $document = array();
    foreach ($entities as $entity) {
        // Notice: No URL Override Field
        $url = url('node/' . $entity->nid);

        //Field Letters
        $letter = fcc_es_get_letter_fields($entity);

        //Notice: No Image
        //Document: Command
        $document['body'][] = array(
          'index' => array(
            '_index' => $index, // TODO make dynamic for property name
            '_type' => $entity->type,
            '_id' => $entity->nid,
          ),
        );
        //Document: Content
        $document['body'][] = array(
          // Moved the author fields into letters array as Letters has two authors
          // One that wrote the letter and one that published it.
          // Default
          'nid' => $entity->nid,
          'title' => $entity->title,
          'uid' => $entity->uid,
          'picture' => $entity->picture,
          'created' => $entity->created,
          'changed' => $entity->changed,
          'revision_timestamp' => $entity->revision_timestamp,
          'vid' => $entity->vid,
          'revision_uid' => $entity->revision_uid,
          'type' => $entity->type,
          'status' => $entity->status,
          'promote' => $entity->promote,
          'sticky' => $entity->sticky,
          'url' => $_SERVER['HTTP_HOST'] . $url, //TODO
          'path' => $entity->path,
          'domains' => array_values($entity->domains),
          'subdomains' => $entity->subdomains,
          'domain_site' => $entity->domain_site,
          //Custom Fields
          // Not Sure if to Include
          //[field_topics_comment_count] ??
          //[field_topics_id] ??
          //[field_topics_url] ??
          'category' => fcc_es_get_taxonomy_terms('field_category', $entity),
          'tags' => fcc_es_get_taxonomy_terms('field_tags', $entity),
          'body' => $entity->body[LANGUAGE_NONE][0],
          'author' => $letter['author'],
          'publisher' => $letter['publisher'],
        );
        unset($letter); // Unset Letter array
    }
    unset($entities); // unset entities

    return $document;
}

/**
 * Converts node entity celebrations to document.
 *
 * @param type $index
 * @param type $offset
 * @param type $batch_size
 * @return array
 */
function _fcc_elasticsearch_indexer_node_celebrations_to_documents($index, $offset, $batch_size) {

    // Load entities
    $entities = _fcc_es_process_node_entities('celebrations', $offset, $batch_size);

    // Build document array
    // Notice: Command and Content arrays must be separated for bulk indexing to work.
    $document = array();
    foreach ($entities as $entity) {

        // Notice: No URL Override Field
        $url = url('node/' . $entity->nid);

        //Field Author
        $author = fcc_es_get_author_fields( $entity );

        //Field Image
        $image_field = $entity->field_image[LANGUAGE_NONE];
        if (!empty($image_field)) {
            $images = array_map(function( $img ) {
                $img_alt = $img['alt'];
                $img_description = $img['description'];
                $img = file_load($img['fid']);
                $img->url = (!empty($image_field[0]['uri']) ) ? file_create_url($img_uri[0]['uri']) : file_create_url($img->uri);
                $img->full_1000 = (!empty($image_field[0]['uri']) ) ? image_style_url('full_1000', $image_field[0]['uri']) : file_create_url($img->uri);
                $img->featured_860 = (!empty($image_field[0]['uri']) ) ? image_style_url('16x9_860', $image_field[0]['uri']) : file_create_url($img->uri);
                $img->featured_620 = (!empty($image_field[0]['uri']) ) ? image_style_url('16x9_620', $image_field[0]['uri']) : file_create_url($img->uri);
                $img->featured_430 = (!empty($image_field[0]['uri']) ) ? image_style_url('16x9_430', $image_field[0]['uri']) : file_create_url($img->uri);
                $img->featured_315 = (!empty($image_field[0]['uri']) ) ? image_style_url('16x9_315', $image_field[0]['uri']) : file_create_url($img->uri);
                $img->square_300 = (!empty($image_field[0]['uri']) ) ? image_style_url('square_300', $image_field[0]['uri']) : file_create_url($img->uri);
                $img->medium = (!empty($image_field[0]['uri']) ) ? image_style_url('medium', $image_field[0]['uri']) : file_create_url($img->uri);
                $img->image_caption = htmlentities($img_alt, ENT_QUOTES, 'UTF-8');
                $img->image_description = $img_description;
                return $img;
            }, $image_field);
        }

        //Document: Command
        $document['body'][] = array(
          'index' => array(
            '_index' => $index, // TODO make dynamic for property name
            '_type' => $entity->type,
            '_id' => $entity->nid,
          ),
        );
        //Document: Content
        $document['body'][] = array(
          // Default
          'nid' => $entity->nid,
          'title' => $entity->title,
          'uid' => $entity->uid,
          'name' => $author['name'],
          'picture' => $entity->picture,
          'created' => $entity->created,
          'changed' => $entity->changed,
          'revision_timestamp' => $entity->revision_timestamp,
          'vid' => $entity->vid,
          'revision_uid' => $entity->revision_uid,
          'type' => $entity->type,
          'status' => $entity->status,
          'promote' => $entity->promote,
          'sticky' => $entity->sticky,
          'path' => $url, //TODO
          'url' => $_SERVER['HTTP_HOST'] . $url, //TODO
          'author' => $author,
          'path' => $entity->path,
          /*
            'domains' => array_values($entity->domains),
            'subdomains' => $entity->subdomains,
            'domain_site' => $entity->domain_site, */
          //Custom Fields
          // This one is pretty simple. :3
          'category' => fcc_es_get_taxonomy_terms('field_category', $entity),
          'body' => $entity->body[LANGUAGE_NONE][0],
          'field_image' => $images,
        );
    }
    unset($entities); // unset entities

    return $document;
}

/**
 * Converts node entity classifieds to document.
 *
 * @param type $index
 * @param type $offset
 * @param type $batch_size
 * @return array
 */
function _fcc_elasticsearch_indexer_node_classified_to_documents($index, $offset, $batch_size) {

    // Load entities
    $entities = _fcc_es_process_node_entities('classified', $offset, $batch_size);

    // Build document array
    // Notice: Command and Content arrays must be separated for bulk indexing to work.
    $document = array();
    foreach ($entities as $entity) {

        // Notice: No URL Override Field
        $url = url('node/' . $entity->nid);

        // Field Classified
        $classified = fcc_es_get_classified_fields($entity);

        //Field Author
        $author = fcc_es_get_author_fields( $entity );

        //Document: Command
        $document['body'][] = array(
          'index' => array(
            '_index' => $index, // TODO make dynamic for property name
            '_type' => $entity->type,
            '_id' => $entity->nid,
          ),
        );
        //Document: Content
        $document['body'][] = array(
          // Default
          'nid' => $entity->nid,
          'title' => $entity->title,
          'uid' => $entity->uid,
          'name' => $author['name'],
          'picture' => $entity->picture,
          'created' => $entity->created,
          'changed' => $entity->changed,
          'revision_timestamp' => $entity->revision_timestamp,
          'vid' => $entity->vid,
          'revision_uid' => $entity->revision_uid,
          'type' => $entity->type,
          'status' => $entity->status,
          'promote' => $entity->promote,
          'sticky' => $entity->sticky,
          'path' => $url, //TODO
          'url' => $_SERVER['HTTP_HOST'] . $url, //TODO
          'author' => $author,
          'path' => $entity->path,
          'domains' => array_values($entity->domains),
          'subdomains' => $entity->subdomains,
          'domain_site' => $entity->domain_site,
          //Custom Fields
          'body' => $entity->body[LANGUAGE_NONE][0],
          'classification' => $classified['classification'],
          'creator_domain' => $classified['creator_domain'],
          'featured' => $classified['featured'],
          'image_url' => $classified['image_url'],
          // Notice: there is already a 'name' field. Rename??
          // Wait... is this being set anywhere?
          //'field_name' => $classified['field_name'],
          'posted' => $classified['posted'],
          'published_to' => $classified['published_to'],
          'type' => $classified['type'],
        );
        unset($classified); // Unset Classified array
    }
    unset($entities); // unset entities

    return $document;
}

/**
 * Converts node entity legals to document.
 *
 * @param type $index
 * @param type $offset
 * @param type $batch_size
 * @return array
 */
function _fcc_elasticsearch_indexer_node_legals_to_documents($index, $offset, $batch_size) {

    // Load entities
    $entities = _fcc_es_process_node_entities('legals', $offset, $batch_size);

    // Build document array
    // Notice: Command and Content arrays must be separated for bulk indexing to work.
    $document = array();
    foreach ($entities as $entity) {


        // Notice: No URL Override Field
        $url = url('node/' . $entity->nid);

        //Field Author
        $author = fcc_es_get_author_fields( $entity );


        //Document: Command
        $document['body'][] = array(
          'index' => array(
            '_index' => $index, // TODO make dynamic for property name
            '_type' => $entity->type,
            '_id' => $entity->nid,
          ),
        );
        //Document: Content
        $document['body'][] = array(
          // Default
          'nid' => $entity->nid,
          'title' => $entity->title,
          'uid' => $entity->uid,
          'name' => $author['name'],
          'picture' => $entity->picture,
          'created' => $entity->created,
          'changed' => $entity->changed,
          'revision_timestamp' => $entity->revision_timestamp,
          'vid' => $entity->vid,
          'revision_uid' => $entity->revision_uid,
          'type' => $entity->type,
          'status' => $entity->status,
          'promote' => $entity->promote,
          'sticky' => $entity->sticky,
          'path' => $url, //TODO
          'url' => $_SERVER['HTTP_HOST'] . $url, //TODO
          'author' => $author,
          'path' => $entity->path,
          'domains' => array_values($entity->domains),
          'subdomains' => $entity->subdomains,
          'domain_site' => $entity->domain_site,
          //Custom Fields
          'body' => $entity->body[LANGUAGE_NONE][0],
        );
    }
    unset($entities); // unset entities

    return $document;
}

/**
 * Process the node entities.
 * @param type $node
 * @param type $offset
 * @param type $batch_size
 * @return type
 */
function _fcc_es_process_node_entities($node_type, $offset, $batch_size) {
    $query = new \EntityFieldQuery();

    $result = $query
        ->entityCondition('entity_type', 'node')
        ->entityCondition('bundle', $node_type)
        ->propertyOrderBy('created', 'DESC')
        ->propertyCondition('status', NODE_PUBLISHED)
        ->range($offset, $batch_size)
        ->execute();

    // Ensure we've got some node results.
    if (!isset($result['node'])) {
        drush_log("No nodes to process.", "ok");
        return;
    }

    // Add the ids with the entity type to the $entities array
    $ids = array_keys($result['node']);

    // Load entities
    $entities = entity_load('node', $ids);
    unset($ids); // unset ids
    unset($result); // close query

    return $entities;
}
