<?php
/**
 * @file
 * commerce_search_api.facetapi_defaults.inc
 */

/**
 * Implements hook_facetapi_default_facet_settings().
 */
function commerce_search_api_facetapi_default_facet_settings() {
  // Don't generate Facets if unnecessary.
  if (!config_get('commerce_search_api.settings', 'commerce_search_api_generate_facets') || !search_api_index_load(COMMERCE_SEARCH_API_INDEX)) {
    return;
  }
  module_load_include('inc', 'commerce_search_api', 'includes/commerce_search_api.callbacks');
  return commerce_search_api_generate_facets(COMMERCE_SEARCH_API_INDEX);
}
