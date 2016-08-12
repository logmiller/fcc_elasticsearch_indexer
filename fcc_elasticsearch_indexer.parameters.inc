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
function _fcc_elasticsearch_indexer_prepare_fields( $params, $node ) { // TODO: Remove if unnecessary

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
 * Prepares the document metadata
 *
 * @param object $node
 * @return array
 */
function _fcc_elasticsearch_indexer_prepare_document_metadata( $node ) {

	// Document Metadata
	$params = array(
		'index' => ES_NODE_ID,
		'type' => $node->type,
		'id' => $node->nid,
		'body' => array(),
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
function _fcc_elasticsearch_indexer_prepare_article( $params, $node ) { // TODO: Move to "Types"?

	$fields = array(
		'nid' => $node->nid,
		'type' => $node->type,
		'title' => $node->title,
		'created' => $node->created,
		'post_date' => date( 'Y-m-d H:i:s', $node->created ),
	);

	foreach ( $fields as $key => $value ) {
		$params['body'][ $key ] = $value;
	}

	//print_r( $params ); // For Debugging. TODO: remove before production
	return $params;
}
