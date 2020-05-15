<?php

class ExportToYml {

  public $yaml;

  public function export() {
    $result = $this->getResult();
    $rows = [];

    foreach ($result as $k => $product) {
      $row = [];

      $row['price'] = $product->field_commerce_price[0]['raw']['amount'];
      $row['price'] = commerce_currency_amount_to_decimal($row['price'], 0);
      $row['currencyId'] = $product->field_commerce_price[0]['raw']['currency_code'];
      $row['name'] = $product->commerce_product_field_data_field_product_title;
      $row['description'] = t('УВАГА !!! Товар є в наявності.') . '<br/>' . t('Якщо ви хочете замовити цей товар, прошу замовте через сайт, або передзвонити по телефону і продиктувати *КАТАЛОЖНИЙ НОМЕР* який знаходиться в цьому оголошені над цим записом. БЕЗ КАТАЛОЖНОГО НОМЕРА ЗАПЧАТИНУ НЕ ЗНАЙДУ. Дякую, прошу ДЗВОНИТИ !!!');

      //picture
      foreach ($product->field_field_image as $field_field_image) {
        $row[] = ['key' => 'picture', 'value' => file_create_url($field_field_image['raw']['uri'])];
      }

      //params
      $row[] = ['key' => 'param', 'attributes' => ['name' => 'Каталожный номер'], 'value' => $product->field_field_serial_namber[0]['raw']['value']];
      $row[] = ['key' => 'param', 'attributes' => ['name' => 'Состояние'], 'value' => 'б/у'];
      $row[] = ['key' => 'param', 'attributes' => ['name' => 'Марка'], 'value' => render($product->field_field_car_brand[0]['rendered'])];
      $row[] = ['key' => 'param', 'attributes' => ['name' => 'Модель'], 'value' => render($product->field_field_car_series[0]['rendered'])];
      $row[] = ['key' => 'param', 'attributes' => ['name' => 'Год выпуска'], 'value' => $product->field_field_year[0]['raw']['value']];
      $row[] = ['key' => 'param', 'attributes' => ['name' => 'available'], 'value' => 'true'];

      $rows[] = ['key' => 'offer', 'attributes' => ['available' => 'true', 'id' => $product->field_field_serial_namber[0]['raw']['value']], 'value' => $row];
    }

    $xmlArray = [
      0 => [
        'key' => 'yml_catalog',
        'attributes' => ['date' => date('Y-m-d H:i'),],
        'value' => [
          'shop' => [
            'currencies' => [],
            'categories' => [],
            'offers' => $rows,
          ],
        ],
      ],
    ];

    $this->toFile($xmlArray);
  }

  public function exportProgress($page = 1) {
    $page = !empty($_REQUEST['page']) ? $_REQUEST['page'] : 1;

    list($result, $progress, $last) = $this->getProgressResult($page);

    $rows = [];
    foreach ($result as $k => $product) {
      $row = [];

      $row['price'] = $product->field_commerce_price[0]['raw']['amount'];
      $row['price'] = commerce_currency_amount_to_decimal($row['price'], 0);
      $row['currencyId'] = $product->field_commerce_price[0]['raw']['currency_code'];
      $row['name'] = $product->commerce_product_field_data_field_product_title;
      $row['description'] = t('УВАГА !!! Товар є в наявності.') . '<br/>' . t('Якщо ви хочете замовити цей товар, прошу замовте через сайт, або передзвонити по телефону і продиктувати *КАТАЛОЖНИЙ НОМЕР* який знаходиться в цьому оголошені над цим записом. БЕЗ КАТАЛОЖНОГО НОМЕРА ЗАПЧАТИНУ НЕ ЗНАЙДУ. Дякую, прошу ДЗВОНИТИ !!!');

      //picture
      foreach ($product->field_field_image as $field_field_image) {
        $row[] = ['key' => 'picture', 'value' => file_create_url($field_field_image['raw']['uri'])];
      }

      //params
      $row[] = ['key' => 'param', 'attributes' => ['name' => 'Каталожный номер'], 'value' => $product->field_field_serial_namber[0]['raw']['value']];
      $row[] = ['key' => 'param', 'attributes' => ['name' => 'Состояние'], 'value' => 'б/у'];
      $row[] = ['key' => 'param', 'attributes' => ['name' => 'Марка'], 'value' => render($product->field_field_car_brand[0]['rendered'])];
      $row[] = ['key' => 'param', 'attributes' => ['name' => 'Модель'], 'value' => render($product->field_field_car_series[0]['rendered'])];
      $row[] = ['key' => 'param', 'attributes' => ['name' => 'Год выпуска'], 'value' => $product->field_field_year[0]['raw']['value']];

      $rows[] = ['key' => 'offer', 'attributes' => ['available' => TRUE, 'id' => $product->field_field_serial_namber[0]['raw']['value']], 'value' => $row];
    }

    if ((int) $page === (int) 1) {
      $this->startToFileInProgress();
    }
    $this->toFileInProgress($rows);

    if($last){
      $this->finishToFileInProgress();
    }

    return $progress;
  }


  /**
   * @return mixed
   */
  public function getResult() {
    return views_get_view_result('auto_pars', 'views_data_export_xml');
  }

  public function getProgressResult($page = 1){
    $name = 'auto_pars';
    $display_id = 'views_data_export_xml';
    $result = [];
    $total_items = $progress = 0;
    $last = FALSE;

    $view = views_get_view($name);
    if (is_object($view)) {

      if (is_string($display_id)) {
        $view->set_display($display_id);
      }
      else {
        $view->init_display();
      }

      $view->init_pager();
      $items_per_page = $view->get_items_per_page();
      $offset = ($page - 1) * $items_per_page;
      $view->set_offset($offset);
      $view->get_total_rows = TRUE;
      $view->pre_execute();
      $view->execute();
      $total_items = $view->query->pager->get_total_items();

      $result = $view->result;
      $progress = ($offset + $items_per_page)/($offset + $items_per_page + $total_items) * 100;
      $last = $total_items < 0 ? TRUE : FALSE ;
    }

    return [$result, $progress, $last];
  }

  public function toFile($yaml) {
    $dir = 'public://yml';
    $xmlstr = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    $xmlstr .= format_xml_elements($yaml);

    file_prepare_directory($dir, FILE_CREATE_DIRECTORY);
    file_save_data($xmlstr,'public://' . 'yml/export.xml', 1);
  }

  public function startToFileInProgress(){
    $xmlstr = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    $xmlstr .= ' <yml_catalog date="' . date('Y-m-d H:i') . '">
<shop> 
<currencies></currencies>
<categories></categories>
<offers>';

    file_prepare_directory($dir, FILE_CREATE_DIRECTORY);
    file_save_data($xmlstr,'public://' . 'yml/export.xml', 1);
  }

  public function toFileInProgress($yaml) {
    $uri = 'public://' . 'yml/export.xml';
    $path = drupal_realpath($uri);

    $xmlstr = format_xml_elements($yaml);

    file_put_contents($path, $xmlstr, FILE_APPEND | LOCK_EX);
  }

  public function finishToFileInProgress(){

    $uri = 'public://' . 'yml/export.xml';
    $path = drupal_realpath($uri);
    $person = '</offers>
</shop>
</yml_catalog>';

    file_put_contents($path, $person, FILE_APPEND | LOCK_EX);
  }
}

