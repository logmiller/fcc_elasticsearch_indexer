<?php

/**
 * @file fcc_elasticsearch_indexer.api.inc
 * Functions for communicating with ElasticSearch
 */

/*------------------------------------------------------------------------------
# ElasticSearch API Functions
------------------------------------------------------------------------------*/

/**
 * Handles bulk indexing post requests using curl
 *
 * @param object $document
 * @return string $response (json)
 */
function fcc_es_post_bulk( $document ) {

	$url = ES_URL . '/_bulk';
	$port = ES_PORT;
	$document = fcc_es_format_bulk_body( $document['body'] );

	$ch = curl_init();
	curl_setopt( $ch, CURLOPT_URL, $url );
	curl_setopt( $ch, CURLOPT_PORT, $port );
	curl_setopt( $ch, CURLOPT_RETURNTRANSFER, 1 );
	curl_setopt( $ch, CURLOPT_CUSTOMREQUEST, 'POST' );
	curl_setopt( $ch, CURLOPT_POSTFIELDS, $document );
	curl_setopt( $ch, CURLOPT_HTTPHEADER, array(
		'Content-Type: application/json',
		'Content-Length: ' . strlen( $document ),
	));
	$response = json_decode( curl_exec( $ch ), true );
	curl_close( $ch );

	unset( $document ); // unset to save memory

	return $response;
}

/**
 * Handles single document indexing post requests using curl
 *
 * @param object $document
 * @return string $response (json)
 */
function fcc_es_post_single( $document ) {

	$url = ES_URL.'/'.$document['index'].'/'.$document['type'].'/'.$document['id'];
	$port = ES_PORT;
	$document = json_encode( $document['body'] );

	$ch = curl_init();
	curl_setopt( $ch, CURLOPT_URL, $url );
	curl_setopt( $ch, CURLOPT_PORT, $port );
	curl_setopt( $ch, CURLOPT_RETURNTRANSFER, 1 );
	curl_setopt( $ch, CURLOPT_CUSTOMREQUEST, 'POST' );
	curl_setopt( $ch, CURLOPT_POSTFIELDS, $document );
	curl_setopt( $ch, CURLOPT_HTTPHEADER, array(
		'Content-Type: application/json',
		'Content-Length: ' . strlen( $document ),
	));
	$response = json_decode( curl_exec( $ch ), true );
	curl_close( $ch );

	unset( $document ); // unset to save memory*/

	return $response;
}


/**
 * Handles single document update post requests using curl
 *
 * @param object $document
 * @return string $response (json)
 */
function fcc_es_delete_single( $document ) {

	$url = ES_URL.'/'.$document['index'].'/'.$document['type'].'/'.$document['id'];
	$port = ES_PORT;

	$ch = curl_init();
	curl_setopt( $ch, CURLOPT_URL, $url );
	curl_setopt( $ch, CURLOPT_PORT, $port );
	curl_setopt( $ch, CURLOPT_RETURNTRANSFER, 1 );
	curl_setopt( $ch, CURLOPT_CUSTOMREQUEST, 'DELETE' );

	$response = json_decode( curl_exec( $ch ), true );
	curl_close( $ch );

	unset( $document ); // unset to save memory*/

	return $response;
}

/**
 * Handles single document query requests using curl
 *
 * @param object $document
 * @return string $response (json)
 */
function fcc_es_query_single( $document ) {

	$url = ES_URL.'/'.$document['index'].'/'.$document['type'].'/'.$document['id'];
	$port = ES_PORT;

	$ch = curl_init();
	curl_setopt( $ch, CURLOPT_URL, $url );
	curl_setopt( $ch, CURLOPT_PORT, $port );
	curl_setopt( $ch, CURLOPT_RETURNTRANSFER, 1 );
	curl_setopt( $ch, CURLOPT_CUSTOMREQUEST, 'GET' );

	$response = json_decode( curl_exec( $ch ), true );
	curl_close( $ch );

	unset( $document ); // unset to save memory*/

	return $response;
}

/*------------------------------------------------------------------------------
# Formatting & Helper Functions
------------------------------------------------------------------------------*/


/**
 * @param string|array|\Traversable $body
 *
 * @return $bulk_body
 */
function fcc_es_format_bulk_body( $body ) {
	if ( empty( $body ) ) {
		return;
	}

	if ( is_array( $body ) === true || $body instanceof \Traversable ) {
		foreach ( $body as $item ) {
			$bulk_body .= fcc_es_serialize_bulk_body( $item ) . "\n";
		}
	} else {
		$bulk_body = $body;
	}

	unset( $body ); // unset to save memory
	return $bulk_body;
}

/**
 * Serialize assoc array into JSON string
 *
 * @param string|array $data Assoc array to encode into JSON
 *
 * @return string
 */
function fcc_es_serialize_bulk_body( $data ) {
	$data = json_encode( $data );
	//$data = json_encode( $data, JSON_PARTIAL_OUTPUT_ON_ERROR ); // TODO this may mask errors, keep?
	if ( $data === '[]' ) {
		return '{}';
	} else {
		return $data;
	}
}
