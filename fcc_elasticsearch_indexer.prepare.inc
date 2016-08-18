<?php

/**
 * @file fcc_elasticsearch_indexer.prepare.inc
 * Indexing Functions for node field parameters
 */

/**
 * Prepares the document metadata for single document indexing
 *
 * @param object $node
 * @return array
 */
function _fcc_elasticsearch_indexer_prepare_document_metadata( $node ) {

	// Document Metadata
	$params = array(
		'index' => ES_INDEX,
		'type' => $node->type,
		'id' => $node->nid,
		//'body' => array(),
	);

	return $params;
}

/**
 * Prepares the document metadata for bulk processing
 *
 * @param object $node
 * @return array
 */
function _fcc_elasticsearch_indexer_prepare_bulk_document_metadata( $node ) {

	// Document Metadata
	$params = array(
		'_index' => ES_INDEX,
		'_type' => $node->type,
		'_id' => $node->nid,
	);

	return $params;
}

/**
 * Validates supported node types
 *
 * @param string $node_type
 * @return bool
 */
function fcc_es_is_supported_node_type( $node_type ) {
	$supported_node_types = array( 'article', 'obituaries', 'letters', 'legals', 'celebrations' );
	if ( in_array( $node_type, $supported_node_types ) ) {
		return true;
	}
}

/**
 * 'Article' Node Type
 *
 * @param array $params
 * @param object $node
 * @return array
 */
function _fcc_elasticsearch_indexer_prepare_article( $node ) {

	$fields = array(
		'nid' => $node->nid,
		'type' => $node->type,
		'post_date' => date( 'Y-m-d H:i:s', $node->created ),
		'title' => $node->title,
		'author' => fcc_es_get_author_fields( $node ),
		'url' => fcc_es_get_url( $node ),
		'search_thumb' => fcc_es_get_search_thumbnail( $node ),
		'uid' => $node->uid,
		'name' => $node->name,
		'picture' => $node->picture,
		'created' => $node->created,
		'published' => fcc_es_get_published_date( $node ),
		'date_terms' => fcc_es_get_date_terms( $node->created ),
		'revision_timestamp' => $node->revision_timestamp,
		'revision_uid' => $node->revision_uid,
		'vid' => $node->vid,
		'status' => $node->status,
		'promote' => $node->promote,
		'sticky' => $node->sticky,
		'newspaper' => fcc_es_get_data_field_value( 'field_newspaper', $node ),
		'parent_domain' => fcc_es_get_parent_domain( $node ),
		'path' => $node->path['source'],
		'domains' => fcc_es_get_domains_field( $node ),
		'subdomains' => $node->subdomains,
		'domain_site' => $node->domain_site,
		'video' => fcc_es_get_file_fields( 'field_video', $node ), // TODO: Verify/Test
		'audio' => fcc_es_get_file_fields( 'field_audio', $node ), // TODO: Verify/Test
		'poll' => fcc_es_get_poll_fields( $node ), // TODO: Verify/Test
		'google_drive_document_id' => fcc_es_get_data_field_value( 'field_google_drive_document_id', $node ),
		'body' => fcc_es_get_body_fields( $node ),
		'category' => fcc_es_get_taxonomy_terms( 'field_category', $node ),
		'tags' => fcc_es_get_taxonomy_terms( 'field_tags', $node ),
		'images' => fcc_es_get_image_fields( $node ),
	);

        // Unset parameters
        unset($node);

	return $fields;
}

/**
 * 'Obituaries' Node Type
 *
 * @param array $params
 * @param object $node
 * @return array
 */
function _fcc_elasticsearch_indexer_prepare_obituaries( $node ) {
	// TODO: Move to "Types"? Remove $params?

	$fields = array(
		'nid' => $node->nid,
		'type' => $node->type,
		'post_date' => date( 'Y-m-d H:i:s', $node->created ),
		'title' => $node->title,
		'name' => $node->name,
		'uid' => $node->uid,
		'picture' => $node->picture,
		'created' => $node->created,
		'published' => fcc_es_get_published_date( $node ),
		'revision_timestamp' => $node->revision_timestamp,
		'revision_uid' => $node->revision_uid,
		'vid' => $node->vid,
		'status' => $node->status,
		'promote' => $node->promote,
		'sticky' => $node->sticky,
		'newspaper' => fcc_es_get_data_field_value( 'field_newspaper', $node ),
		'parent_domain' => null, // TODO
		'domains' => fcc_es_get_domains_field( $node ),
		'subdomains' => $node->subdomains,
		'domain_site' => $node->domain_site,
		'url' => fcc_es_get_url( $node ),
		'path' => $node->path['source'], // TODO: Verify/Test
                'body' => fcc_es_get_body_fields( $node ),
                'field_image' => fcc_es_get_image_fields( $node ), // TODO: Verify/Test
                'funeral_home' => null, // TODO: Fix
                'lifespan' => null, // TODO: Fix
                'logo' => null, // TODO: Fix
                'location' => null, // TODO: Fix
	);

	// single post only (if not bulk)
	/*foreach ( $fields as $key => $value ) {
		$params['body'][ $key ] = $value;
	}*/

	//print_r( $fields ); // For Debugging. TODO: remove before production
	return $fields;
}


/**
 * 'Letters' Node Type
 *
 * @param array $params
 * @param object $node
 * @return array
 */
function _fcc_elasticsearch_indexer_prepare_letters( $node ) {
	// TODO: Move to "Types"? Remove $params?

	$fields = array(
          	'nid' => $node->nid,
		'type' => $node->type,
		'post_date' => date( 'Y-m-d H:i:s', $node->created ),
		'title' => $node->title,
		'name' => $node->name,
		'uid' => $node->uid,
		'picture' => $node->picture,
		'created' => $node->created,
		'published' => fcc_es_get_published_date( $node ),
		'revision_timestamp' => $node->revision_timestamp,
		'revision_uid' => $node->revision_uid,
		'vid' => $node->vid,
		'status' => $node->status,
		'promote' => $node->promote,
		'sticky' => $node->sticky,
		'newspaper' => fcc_es_get_data_field_value( 'field_newspaper', $node ),
		'parent_domain' => null, // TODO
		'domains' => fcc_es_get_domains_field( $node ),
		'subdomains' => $node->subdomains,
		'domain_site' => $node->domain_site,
		'url' => fcc_es_get_url( $node ),
		'path' => $node->path['source'], // TODO: Verify/Test
                'category' => fcc_es_get_taxonomy_terms( 'field_category', $node ), // TODO: Fix
                'tags' => fcc_es_get_taxonomy_terms( 'field_tags', $node ), // TODO: Fix
                'body' => fcc_es_get_body_fields( $node ),
                'author' => null, //TODO: Fix
                'publisher' => null, // TODO: Fix
	);

	// single post only (if not bulk)
	/*foreach ( $fields as $key => $value ) {
		$params['body'][ $key ] = $value;
	}*/

	//print_r( $fields ); // For Debugging. TODO: remove before production
	return $fields;
}

/**
 * 'Legals' Node Type
 *
 * @param array $params
 * @param object $node
 * @return array
 */
function _fcc_elasticsearch_indexer_prepare_legals( $node ) {
	// TODO: Move to "Types"? Remove $params?

	$fields = array(
		'nid' => $node->nid,
		'type' => $node->type,
		'post_date' => date( 'Y-m-d H:i:s', $node->created ),
		'title' => $node->title,
		'name' => $node->name,
		'uid' => $node->uid,
		'picture' => $node->picture,
		'created' => $node->created,
		'published' => fcc_es_get_published_date( $node ),
		'revision_timestamp' => $node->revision_timestamp,
		'revision_uid' => $node->revision_uid,
		'vid' => $node->vid,
		'status' => $node->status,
		'promote' => $node->promote,
		'sticky' => $node->sticky,
		'newspaper' => fcc_es_get_data_field_value( 'field_newspaper', $node ),
		'parent_domain' => null, // TODO
		'domains' => fcc_es_get_domains_field( $node ),
		'subdomains' => $node->subdomains,
		'domain_site' => $node->domain_site,
		'url' => fcc_es_get_url( $node ),
		'path' => $node->path['source'], // TODO: Verify/Test
                'body' => fcc_es_get_body_fields( $node ),
	);

	// single post only (if not bulk)
	/*foreach ( $fields as $key => $value ) {
		$params['body'][ $key ] = $value;
	}*/

	//print_r( $fields ); // For Debugging. TODO: remove before production
	return $fields;
}

/**
 * 'Celebrations' Node Type
 *
 * @param array $params
 * @param object $node
 * @return array
 */
function _fcc_elasticsearch_indexer_prepare_celebrations( $node ) {
	// TODO: Move to "Types"? Remove $params?

	$fields = array(
		'nid' => $node->nid,
		'type' => $node->type,
		'post_date' => date( 'Y-m-d H:i:s', $node->created ),
		'title' => $node->title,
		'name' => $node->name,
		'uid' => $node->uid,
		'picture' => $node->picture,
		'created' => $node->created,
		'published' => fcc_es_get_published_date( $node ),
		'revision_timestamp' => $node->revision_timestamp,
		'revision_uid' => $node->revision_uid,
		'vid' => $node->vid,
		'status' => $node->status,
		'promote' => $node->promote,
		'sticky' => $node->sticky,
		'newspaper' => fcc_es_get_data_field_value( 'field_newspaper', $node ),
		'parent_domain' => null, // TODO
		'domains' => fcc_es_get_domains_field( $node ),
		'subdomains' => $node->subdomains,
		'domain_site' => $node->domain_site,
		'url' => fcc_es_get_url( $node ),
		'path' => $node->path['source'], // TODO: Verify/Test
                'category' => fcc_es_get_taxonomy_terms( 'field_category', $node ), // TODO: Fix
                'body' => fcc_es_get_body_fields( $node ),
                'field_image' => fcc_es_get_image_fields( $node ), // TODO: Verify/Test*/
	);

	// single post only (if not bulk)
	/*foreach ( $fields as $key => $value ) {
		$params['body'][ $key ] = $value;
	}*/

	//print_r( $fields ); // For Debugging. TODO: remove before production
	return $fields;
}
