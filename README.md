# FCC ElasticSearch Indexer
ElasticSearch Indexing module for Drupal 7

### Setup
Custer settings are stored as variables in Drupal.
- `drush vset es_index fccnn` Set the Cluster Index Name
- `drush vset es_index {{cluster url}}` Set the Cluster Index URL

### Indexing

> `drush es-i [node_type] [limit] [offset] [batch_size]`

**Arguments:**
- `node_type` The node type being inserted.
- `limit` The number of items to index (index's cron batch size items per run). Set to 0 to index all items. Defaults to 0 (index all).
- `offset` The number to items to skip. Defaults to 0 (index from beginning).
- `batch_size` The number of items to index per batch run. Set to 0 to index all items at once. Defaults to the index's cron batch size.

**Examples:**
- `drush elastic-search-index` Index items for all of the elastic search indexes.
- `drush es-i` Alias to index items for all of the elastic search indexes.
- `drush es-i article 100` Index a maximum number of `[limit]` items (index's batch size items per batch run) by the node type `[node_type]`.
- `drush es-i article 100 10` Index a maximum number of `[limit]` items by offset `[offset]` by the node type `[node_type]`.
- `drush es-i article 0 100` Index all items with offset `[offset]`.
- `drush es-i article 0 10 100` Index all items from offset !offset with !batch_size items per batch run.
