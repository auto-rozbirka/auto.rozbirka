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
      $row['description'] = $product->commerce_product_field_data_field_product_title;

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

  /**
   * @return mixed
   */
  public function getResult() {
    return views_get_view_result('auto_pars', 'views_data_export_xml');
  }

  public function toFile($yaml) {
    $dir = 'public://yml';
    $xmlstr = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    $xmlstr .= format_xml_elements($yaml);

    file_prepare_directory($dir, FILE_CREATE_DIRECTORY);
    file_save_data($xmlstr,'public://' . 'yml/export.xml', 1);
  }

}

