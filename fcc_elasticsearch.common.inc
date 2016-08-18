<?php

/**
 * TODO: There may be a more Drupal way to do this.
 * Entity load process stripped of all its caching layer.
 */

/**
 * Override entity_load().
 */
function _fcc_es_entity_load($entity_type, $ids = FALSE) {
    $entity = _fcc_es_entity_get_controller($entity_type)->load($ids);
    return $entity;
}

/**
 * Overrides entity_get_controller().
 */
function _fcc_es_entity_get_controller($entity_type) {
    $type_info = _fcc_es_entity_get_info($entity_type);
    $class = $type_info['controller class'];
    $controllers[$entity_type] = new $class($entity_type);
    unset($type_info);

    return $controllers[$entity_type];
}

/**
 * Overrides entity_get_info().
 */
function _fcc_es_entity_get_info($entity_type = NULL) {
    $entity_info = module_invoke_all('entity_info');
    // Merge in default values.
    foreach ($entity_info as $name => $data) {
        $entity_info[$name] += array(
          'fieldable' => FALSE,
          'controller class' => 'DrupalDefaultEntityController',
          'static cache' => FALSE,
          'field cache' => FALSE,
          'load hook' => $name . '_load',
          'bundles' => array(),
          'view modes' => array(),
          'entity keys' => array(),
          'translation' => array(),
        );
        $entity_info[$name]['entity keys'] += array(
          'bundle' => '',
        );
        foreach ($entity_info[$name]['view modes'] as $view_mode => $view_mode_info) {
            $entity_info[$name]['view modes'][$view_mode] += array(
              'custom settings' => FALSE,
            );
        }
        // If no bundle key is provided, assume a single bundle, named after
        // the entity type.
        if (empty($entity_info[$name]['entity keys']['bundle']) && empty($entity_info[$name]['bundles'])) {
            $entity_info[$name]['bundles'] = array($name => array('label' => $entity_info[$name]['label']));
        }
        // Prepare entity schema fields SQL info for
        // DrupalEntityControllerInterface::buildQuery().
        if (isset($entity_info[$name]['base table'])) {
            $entity_info[$name]['base table field types'] = drupal_schema_field_types($entity_info[$name]['base table']);
            $entity_info[$name]['schema_fields_sql']['base table'] = drupal_schema_fields_sql($entity_info[$name]['base table']);
        }
    }

    if (empty($entity_type)) {
        return $entity_info;
    }
    elseif (isset($entity_info[$entity_type])) {
        return $entity_info[$entity_type];
    }
}
