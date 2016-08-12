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

function _fcc_elasticsearch_indexer_prepare_article( $params, $node ) {
	// Add the simple properties
	$wanted = array( 'nid', 'title', 'uid', 'name', 'picture', 'created', 'changed', 'revision_timestamp', 'vid', 'revision_uid', 'type', 'status', 'promote', 'sticky', 'domain_site' );
	$exist = array_filter( $wanted, function( $property ) use ( $node ) {
		return property_exists( $node, $property );
	});
	foreach ( $exist as $field ) {
		$params['body'][ $field ] = $node->{$field};
	}

	// Add the article 'URL' link // NOTE leave protocol off for now
	if ( ! empty( $node->field_override_url ) ) {
		$url = $node->field_override_url[ $node->language ][0]['value']; // URL Override
	} else {
		$url = url( 'node/' . $nid ); // No URL Override
	}
	if ( $url ) {
		$params['body']['path'] = $url;
		$params['body']['url'] = $_SERVER['HTTP_HOST'] . $url;
	}

	// Add the 'Published' date field (if it exists)
	if ( $node->workbench_moderation->published ) {
		//$published = date( 'Y-m-d', $node->workbench_moderation->published->timestamp );
		$published = $node->workbench_moderation->published->timestamp;
		$params['body']['published'] = $published;
	}

	// Add the 'Author' fields // TODO Expand This
	$author = fcc_es_get_author_fields( $nid, $uid );
	if ( $author ) {
		$params['body']['author'] = $author;
	}

	// Add the 'Category' field (if it exists) // TODO Expand This
	$category_field = isset( $node->field_category ) ? field_get_items( 'node', $node, 'field_category' ) : false;
 if ( $category_field ) {
		$params['body']['category'] = NULL;
	}

	// Add the 'Tags' field (if it exists) // TODO Expand This
	$tags_field = isset( $node->field_tags ) ? field_get_items( 'node', $node, 'field_tags' ) : false;
	if ( $tags_field ) {
		$params['body']['tags'] = NULL;
	}

	// Add the 'Body' field (if it exists)
	$body_field = isset( $node->body ) ? field_get_items( 'node', $node, 'body' ) : false;
	if ( $body_field ) {
		$params['body']['body'] = $body_field;
	}

	// Add the 'Image' field (if it exists)
	$image_field = isset( $node->field_image ) ? field_get_items( 'node', $node, 'field_image' ) : false;
	if ( $image_field ) {
		$params['body']['field_image'] = array_map( function( $img ) {
			$img_alt = $img['alt'];
			$img_description = $img['description'];
			$img = file_load( $img['fid'] );
			$img->url = ( ! empty( $image_field[0]['uri'] ) ) ? file_create_url( $img_uri[0]['uri'] ) : file_create_url( $img->uri );
			$img->full_1000 = ( ! empty( $image_field[0]['uri'] ) ) ? image_style_url( 'full_1000', $image_field[0]['uri'] ) : file_create_url( $img->uri );
			$img->featured_860 = ( ! empty( $image_field[0]['uri'] ) ) ? image_style_url( '16x9_860', $image_field[0]['uri'] ) : file_create_url( $img->uri );
			$img->featured_620 = ( ! empty( $image_field[0]['uri'] ) ) ? image_style_url( '16x9_620', $image_field[0]['uri'] ) : file_create_url( $img->uri );
			$img->featured_430 = ( ! empty( $image_field[0]['uri'] ) ) ? image_style_url( '16x9_430', $image_field[0]['uri'] ) : file_create_url( $img->uri );
			$img->featured_315 = ( ! empty( $image_field[0]['uri'] ) ) ? image_style_url( '16x9_315', $image_field[0]['uri'] ) : file_create_url( $img->uri );
			$img->square_300 = ( ! empty( $image_field[0]['uri'] ) ) ? image_style_url( 'square_300', $image_field[0]['uri'] ) : file_create_url( $img->uri );
			$img->medium = ( ! empty( $image_field[0]['uri'] ) ) ? image_style_url( 'medium', $image_field[0]['uri'] ) : file_create_url( $img->uri );
			$img->image_caption = htmlentities( $img_alt, ENT_QUOTES, 'UTF-8' );
			$img->image_description = $img_description;
			return $img;
		}, $image_field);
	}

	return $params;
}
