<?php

/**
 * @file fcc_elasticsearch_indexer.parameters.inc
 * Indexing Functions for node field parameters
 */

/**
 * Helper routing function to prepare field parameters based on node type.
 *
 * @param array $params
 * @param object $node
 * @return array
 */
function _fcc_elasticsearch_indexer_prepare_fields( $params, $node ) {
	// TODO: Remove if unnecessary

	if ( 'article' == $node->type ) {
		$params = _fcc_elasticsearch_indexer_prepare_article( $params, $node );
	} else {
		drupal_set_message( t( 'There was a problem saving this node to Elastic - only the article node type is currently supported.' ) );
		if ( drupal_is_cli() && function_exists( 'drush_main' ) ) { // Code is being run from drush.
			drush_set_error( 'Error: only the article node type is currently supported.' );
			return;
		}
	}

	return $params;
}

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
 * 'Article' Node Type
 *
 * @param array $params
 * @param object $node
 * @return array
 */
function _fcc_elasticsearch_indexer_prepare_article( $node ) {
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
		'url' => fcc_es_get_url( $node ), // TODO parent_url
		'path' => $node->path['source'], // TODO: Verify/Test
		'video' => fcc_es_get_file_fields( 'field_video', $node ), // TODO: Verify/Test
		'audio' => fcc_es_get_file_fields( 'field_audio', $node ), // TODO: Verify/Test
		'poll' => fcc_es_get_poll_fields( $node ), // TODO: Verify/Test
		'google_drive_document_id' => fcc_es_get_data_field_value( 'field_google_drive_document_id', $node ),
		'body' => fcc_es_get_body_fields( $node ),
		'category' => fcc_es_get_taxonomy_terms( 'field_category', $node ), // TODO: Fix
		'tags' => fcc_es_get_taxonomy_terms( 'field_tags', $node ), // TODO: Fix
		'author' => fcc_es_get_author_fields( $node ),
		'field_image' => fcc_es_get_image_fields( $node ), // TODO: Verify/Test
	);

	// single post only (if not bulk)
	/*foreach ( $fields as $key => $value ) {
		$params['body'][ $key ] = $value;
	}*/

	//print_r( $fields ); // For Debugging. TODO: remove before production
	return $fields;
}
