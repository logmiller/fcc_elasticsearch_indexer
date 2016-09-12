<?php

/**
 * @file fcc_upgrade_helper.drush.inc
 * Drush Commands
 */

# Load Includes
include_once 'progress.drush.inc';

/**
 * Implements hook_drush_command().
 */
function fcc_elasticsearch_indexer_drush_command() {
    $items = array();
    $items['elastic-search-index'] = array(
      'description' => 'Index items for one or all of the elastic search indexes.',
      'examples' => array(
        'drush elastic-search-index' => dt('Index items for all of the elastic search indexes.'),
        'drush es-i' => dt('Alias to index items for all of the elastic search indexes.'),
        'drush es-i article 100' => dt("Index a maximum number of !limit items (index's cron batch size items per batch run) by the node type !type.", array('!limit' => 100, '!type' => 'article')),
        'drush es-i article 100 10' => dt("Index a maximum number of !limit items by offset !offset by the node type !type.", array('!limit' => 100, '!offset' => 10, '!type' => 'article')),
        'drush es-i article 0 100' => dt("Index all items with offset !offset.", array('!offset' => 10)),
        'drush es-i article 0 10 100' => dt("Index all items from offset !offset with !batch_size items per batch run.", array('!offset'=>'10','!batch_size' => 100)),
      ),
      'arguments' => array(
        'node_type' => dt("The node type being inserted."),
        'limit' => dt("The number of items to index (index's cron batch size items per run). Set to 0 to index all items. Defaults to 0 (index all)."),
        'offset' => dt("The number to items to skip. Defaults to 0 (index from beginning)."),
        'batch_size' => dt("The number of items to index per batch run. Set to 0 to index all items at once. Defaults to the index's cron batch size."),
      ),
      'aliases' => array('es-i'),
    );

    return $items;
}

/**
 * Index items.
 *
 * @param string $node_type
 *   The node type of the items being indexed.
 * @param integer $limit
 *   Maximum number of items to index.
 * @param integer $batch_size
 *   Number of items to index per batch.
 */
function drush_fcc_elasticsearch_indexer_elastic_search_index($node_type = NULL, $limit = NULL, $offset = 0, $batch_size = 100) {
    if (elasticsearch_index_drush_static(__FUNCTION__)) {
        return;
    }
    // Check if node type is of supported node types
    if (fcc_es_is_supported_node_type($node_type)) {
        // Get total count if limit isn't set
        if (empty($limit)) {
            $query = new \EntityFieldQuery();
            $limit = $query
                    ->entityCondition('entity_type', 'node')
                    ->entityCondition('bundle', $node_type)
                    ->propertyCondition('status', NODE_PUBLISHED)
                    ->count()->execute();
            unset($query); // Unset query
        }
        _drush_elastic_search_batch_indexing_create($node_type, $limit, $offset, $batch_size);
    }
    else {
        drupal_set_message(t('There was a problem saving this node to Elastic - the node type used is not supported.'));
        if (drupal_is_cli() && function_exists('drush_main')) { // Code is being run from drush.
            drush_set_error('Error: the node type used is currently not supported.');
            return;
        }
    }
}

/**
 * Creates and sets a batch for indexing items for a particular index.
 *
 * @param $client
 *   The elastic search cluster.
 * @param string $node_type
 *   The node type of the items being indexed.
 * @param int $limit
 *   (optional) The maximum number of items to index, or NULL to index all
 *   items.
 * @param int $batch_size
 *   (optional) The number of items to index per batch.
 *
 * @return bool
 *   TRUE if batch was created, FALSE otherwise.
 */
function _drush_elastic_search_batch_indexing_create($node_type, $limit, $offset, $batch_size) {
    // Get the number of remaining items to index.
    try {
        // ELASTICSEARCH BULK INDEX
        $bar = drush_create_progress();
        //Create index and mapping
        for ($count = 0; $count <= $limit; $count++) {
            if ($count % $batch_size == 0) { // Every n documents stop and send the bulk request
                // Build elastic search query
                $document = _fcc_elasticsearch_indexer_node_article_to_documents($node_type, $offset, $batch_size);
                $responses = fcc_es_post_bulk($document); // send the bulk index request

                // indexing success/fail response from elasticsearch
                if ($responses['errors'] > 0) {
									_fcc_elasticsearch_set_error_log($responses);
                }

                $offset += $batch_size;
                unset($document);  // erase the old bulk request
                unset($responses); // unset  to save memory
                //print 'count:' . $count . "\n";
                $bar->setProgress($limit, $count);
            }
        }
        $bar->end();
    }
    catch (Exception $ex) {
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
