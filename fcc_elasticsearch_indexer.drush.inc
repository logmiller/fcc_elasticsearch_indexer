<?php

/**
 * @file fcc_upgrade_helper.drush.inc
 * TODO: Enter file description here.
 */
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
function fcc_elasticsearch_indexer_drush_command() {
  $items = array();
  $items['elastic-search-index'] = array(
    'description' => 'Index items for one or all of the elastic search indexes.',
    'examples' => array(
      'drush elastic-search-index' => dt('Index items for all of the elastic search indexes.'),
      'drush es-i' => dt('Alias to index items for all of the elastic search indexes.'),
      'drush es-i 100' => dt("Index a maximum number of !limit items (index's cron batch size items per batch run) for the index name !name.", array('!limit' => 100, '!name' => 'fccnn')),
      'drush es-i 100 10' => dt("Index a maximum number of !limit items (!batch_size items per batch run) for the index name !name.", array('!limit' => 100, '!batch_size' => 10, '!name' => 'fccnn')),
      'drush es-i 0 100' => dt("Index all items of all indexes with !batch_size items per batch run.", array('!batch_size' => 100)),
    ),
    'arguments' => array(
      'limit' => dt("The number of items to index (index's cron batch size items per run). Set to 0 to index all items. Defaults to 0 (index all)."),
      'batch_size' => dt("The number of items to index per batch run. Set to 0 to index all items at once. Defaults to the index's cron batch size."),
    ),
    'aliases' => array('es-i'),
  );

  return $items;
}

/**
 * Index items.
 *
 * @param string $index
 *   The index name for which items should be indexed.
 * @param integer $limit
 *   Maximum number of items to index.
 * @param integer $batch_size
 *   Number of items to index per batch.
 */
function drush_fcc_elasticsearch_indexer_elastic_search_index($limit = NULL, $batch_size = 5) {
  if (elasticsearch_index_drush_static(__FUNCTION__)) {
    return;
  }

  // Get total count if limit isn't set
  if (empty($limit)) {
    $query = new \EntityFieldQuery();
    $limit = $query
            ->entityCondition('entity_type', 'node')
            ->entityCondition('bundle', 'article')
            ->count()->execute();
  }

  $client = elasticsearch_connector_get_client_by_id(DEFAULT_CLUSTER_ID);
  if (empty($client)) {
    return;
  }
  _drush_elastic_search_batch_indexing_create($client, 'fccnn', $limit, $batch_size);
}

/**
 * Creates and sets a batch for indexing items for a particular index.
 *
 * @param $client
 *   The elastic search cluster.
 * @param int $limit
 *   (optional) The maximum number of items to index, or NULL to index all
 *   items.
 * @param int $batch_size
 *   (optional) The number of items to index per batch.
 *
 * @return bool
 *   TRUE if batch was created, FALSE otherwise.
 */
function _drush_elastic_search_batch_indexing_create($client, $index, $limit, $batch_size) {
  // Get the number of remaining items to index.
  try {
// ELASTICSEARCH BULK INDEX
    //Create index and mapping
    $offset = 0;
    for ($count = 0; $count <= $limit; $count++) {
      if ($count % $batch_size == 0) {   // Every n documents stop and send the bulk request
        $document = _fcc_elasticsearch_indexer_node_entity_to_documents($index, $offset, $batch_size);
        $responses = $client->bulk($document);
        $offset = $count;
        unset($document);  // erase the old bulk request
        unset($responses); // unset  to save memory
      }
    }
  } catch (Exception $ex) {
    drush_log($ex->getMessage(), 'error');
  }
}

/**
 * Does a static lookup to prevent Drush 4 from running twice.
 *
 * @param string $function
 *   The Drush function being called.
 *
 * @return bool
 *   TRUE if the function was already called in this Drush execution, FALSE
 *   otherwise.
 *
 * @see http://drupal.org/node/704848
 */
function elasticsearch_index_drush_static($function) {
  static $index = array();
  if (isset($index[$function])) {
    return TRUE;
  }
  $index[$function] = TRUE;
  return FALSE;
}
