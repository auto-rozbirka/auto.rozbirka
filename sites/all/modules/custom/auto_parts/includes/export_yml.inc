<?php


class ExportToYml {

  public $yaml;

  public function __construct() {

  }

  public function export() {
    $result = $this->getResult();
    $rows = [];


    foreach ($result as $k => $product) {
      $row = [];
      $row['price'] = $product->field_commerce_price[0]['raw']['amount'];
      $row['currencyId'] = $product->field_commerce_price[0]['raw']['currency_code'];
      $row['picture'] = file_create_url($product->field_field_image[0]['raw']['uri']);
      $row['name'] = $product->node_title;
      $row['description'] = $product->node_title;

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

