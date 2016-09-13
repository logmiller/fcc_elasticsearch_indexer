<?php

/**
 * cURL commands for ElasticSearch data retrieval.
 */

/**
 * Request node from ElasticSearch.
 * @param type $nids
 *  An array of node ids.
 * @param type $fields
 *  An array of fields, or FALSE to load all the fields.
 * @return type
 */
function _fcc_es_connect_post_nodes_by_id($nids, $fields = FALSE) {

    //ELASTICSEARCH: QUERY
    if (empty($nids)) {
        return;
    }
    $query_filter = count($nids) > 1 ? 'terms' : 'term';

    $postfields = json_encode(array(
      '_source' => ($fields) ? $fields : [],
      'query' => array(
        $query_filter => array(
          '_id' => $nids,
    ))));

    return _fcc_es_connect_post_curl($postfields);
}

/**
 * Request the latest nodes from ElasticSearch by type.
 *
 * @param type $type
 * @param type $limit
 * @param type $offset
 * @param type $fields
 * @return type
 */
function _fcc_es_connect_post_latest_nodes_by_type($type = 'article', $limit = 15, $offset = 0, $fields = FALSE) {

    $postfields = json_encode(array(
      '_source' => ($fields) ? $fields : [],
      'sort' => array('published' => array('order' => 'desc')),
      'from' => $offset,
      'size' => $limit,
      'query' => array(
        'constant_score' => array(
          'filter' => array(
            'bool' => array(
              'must' => array(
                'term' => array('_type' => $type),
                'term' => array('domains' => 15),
              ),
            ))),
      ),
    ));

    return _fcc_es_connect_post_curl($postfields);
}

/**
 * Request the latest nodes from ElasticSearch by category id.
 *
 * @param type $tid
 * @param type $limit
 * @param type $offset
 * @param type $fields
 * @return type
 */
function _fcc_es_connect_post_latest_nodes_by_tid($tid = 1, $limit = 15, $offset = 0, $fields = FALSE) {
    $postfields = json_encode(array(
      '_source' => ($fields) ? $fields : [],
      'sort' => array('published' => array('order' => 'desc')),
      'from' => $offset,
      'size' => $limit,
      'query' => array(
        'constant_score' => array(
          'filter' => array(
            'bool' => array(
              'must' => array(
                'term' => array('category.tid' => $tid),
                'term' => array('domains' => 15),
              ),
            ))),
      ),
    ));

    return _fcc_es_connect_post_curl($postfields);
}

/**
 * Construct queries for Elastic Search.
 */
function _fcc_es_connect_make_url() {

    $es_url = variable_get('es_url', '');
    if (!$es_url) {
        $es_url = ES_URL;
    }

    $es_index = variable_get('es_index', '');
    if (!$es_index) {
        $es_index = ES_INDEX;
    }

    $search = array(
      'url' => $es_url,
      'index' => $es_index,
    );

    $query = $search['url'] . '/' . $search['index'] . '/_search';

    return $query;
}

/**
 * cURL post command for ElasticSearch.
 * @param type $postfields
 * @return type
 */
function _fcc_es_connect_post_curl($postfields) {
    $curl = curl_init();

    curl_setopt_array($curl, array(
      CURLOPT_PORT => "9200",
      CURLOPT_URL => _fcc_es_connect_make_url(),
      CURLOPT_RETURNTRANSFER => true,
      CURLOPT_ENCODING => "",
      CURLOPT_MAXREDIRS => 10,
      CURLOPT_TIMEOUT => 30,
      CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
      CURLOPT_CUSTOMREQUEST => "POST",
      CURLOPT_POSTFIELDS => $postfields,
      CURLOPT_HTTPHEADER => array(
        "cache-control: no-cache",
      ),
    ));

    $response = curl_exec($curl);
    $err = curl_error($curl);

    curl_close($curl);

    if ($err) {
        echo "cURL Error #:" . $err;
    }
    else {
        return json_decode($response);
    }
}
