<?php
/**
 * @file
 * The commerce_search_api_fancy widget plugin class.
 */

/**
 * Widget that renders taxonomy terms as fancy attributes
 */
class CommerceSearchApiFancy extends FacetapiWidgetLinks {

  /**
   * Overrides FacetapiWidget:Links:settingsForm().
   */
  function settingsForm(&$form, &$form_state) {
    parent::settingsForm($form, $form_state);
    $form['widget']['widget_settings']['links'][$this->id]['display_count'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display the number of matching items'),
      '#default_value' => !empty($this->settings->settings['display_count']) ? $this->settings->settings['display_count'] : FALSE,
      '#description' => t('If checked, the number of matching items will be output with the color square.'),
    );
  }

  /**
   * Overrides FacetapiWidgetLinks::buildListItems().
   *
   * Add our css file and call our theme function to theme taxonomy terms as
   * fancy attributes.
   */
  function buildListItems($build) {
    $facet_info = $this->facet->getFacet();
    if ($facet_info['field type'] == 'taxonomy_term' && module_exists('commerce_fancy_attributes')) {
      $terms = taxonomy_term_load_multiple(array_keys($build));
      $first_term = reset($terms);
      $bundle = $first_term->vocabulary_machine_name;
      $fancy_attribute_field = FALSE;
      // Loop over the field instances to find an instance that use the
      // fancy attributes formatter, select the first field found.
      // TODO: Probably expose a setting for that.
      foreach (field_info_instances('taxonomy_term', $bundle) as $field_name => $instance) {
        if ($instance['display']['add_to_cart_form']['type'] == 'commerce_fancy_attributes_color') {
          $fancy_attribute_field = $field_name;
          break;
        }
      }
      // If we've found at least one field that uses the fancy attributes formatter.
      if ($fancy_attribute_field) {
        drupal_add_css(drupal_get_path('module', 'commerce_search_api') . '/commerce_search_api.css');
        foreach ($build as $value => &$item) {
          $term_wrapper = entity_metadata_wrapper('taxonomy_term', $terms[$value]);
          if (!isset($term_wrapper->{$fancy_attribute_field})) {
            continue;
          }
          $item['#html'] = TRUE;
          $variables['hex'] = $term_wrapper->{$fancy_attribute_field}->value();

          if (!empty($this->settings->settings['display_count'])) {
            $variables['title'] = $item['#count'];
          }

          $item['#markup'] = theme('commerce_search_api_fancy_attributes_color', $variables);
          $item['#count'] = NULL;
        }
      }
    }
    $items = parent::buildListItems($build);
    return $items;
  }

}
