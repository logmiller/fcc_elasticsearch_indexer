<?php

/**
 * cURL commands for ElasticSearch data retrieval.
 */
/*

  /**
 * Request node from ElasticSearch.
 * @param type $nids
 *  An array of node ids.
 * @param type $fields
 *  An array of fields, or FALSE to load all the fields.
 * @return type
 */
function _fcc_es_connect_search_nodes_by_id($nids, $fields = FALSE) {

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
        return $response;
    }
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
