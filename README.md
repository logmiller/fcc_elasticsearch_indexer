# FCC ElasticSearch Indexer
ElasticSearch Indexing module for Drupal 7

### Setup
Custer settings are stored as variables in Drupal.

#### Get the cluster variable settings:
`drush php-eval 'print_r(_fcc_es_get_elasticsearch_presets());'` - Returns a settings array.

#### Set, update or override the cluster variable settings:
- `drush vset es_index {{index name}}` Set the Cluster Index Name
- `drush vset es_url {{cluster url}}` Set the Cluster Index URL
- `drush vset es_port {{cluster port}}` Set the Cluster Index URL

#### Cluster Environment Default Settings

**Production**:
- Index Name: `fccnn`
- Port: {{port}}
- URL: {{url}}

**Staging**:
- Index Name: `fccnn-staging`
- Port: {{port}}
- URL: {{url}}

**Testing / Upgrade Testing**:
- Index Name: `fccnn-testing`
- Port: {{port}}
- URL: {{url}}

**Dev / Local**:
- Index Name: `fccnn`
- Port: `9200`
- URL: `http://localhost:9200`

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
