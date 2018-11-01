<?php

require_once drupal_get_path('module', 'auto_parts') . '/library/vendor/autoload.php';

use PhpOffice\PhpSpreadsheet\Spreadsheet;
use PhpOffice\PhpSpreadsheet\Writer\Xlsx;

class ExportToExcel {

  /**
   * @var \PhpOffice\PhpSpreadsheet\Spreadsheet
   */
  public $spreadsheet;

  public $xlsx;

  public function __construct() {
    ini_set('memory_limit', '512M');
    ini_set('max_execution_time', 300);

    $this->spreadsheet = new Spreadsheet();
  }

  public function export() {

//    $dir = 'public://yml';
//    file_prepare_directory($dir, FILE_CREATE_DIRECTORY);
//    $file = file_save_data('asd','public://' . 'yml/export.yml', 1);

    $view = $this->viewExecute();

    $cel = 1;
    foreach ($view->result as $row) {
      $currentColumn = "A";
      foreach ($view->field as $field => $handler_field) {
        $value = '';
        if ($handler_field->allow_advanced_render() && method_exists($handler_field, 'render_item')) {
          $raw_items = $handler_field->get_items($row);
          // If there are no items, set the original value to NULL.

          if (!empty($raw_items)) {
            $values = [];
            foreach ($raw_items as $raw_item) {
              $values[] = drupal_render($raw_item);
            }
            $value = implode(', ', $values);
          }
        }
        else {
          $value = $handler_field->get_value($row);
        }

        $this->setCellValue($currentColumn . $cel, $value);

        $currentColumn++;
      }

      if ($cel > 1){
        break;
      }
    }
//    $this->toExel();

    return 222;
    $spreadsheet = new Spreadsheet();
    $sheet = $spreadsheet->getActiveSheet();
    $sheet->setCellValue('A1', 'Hello World !');

    $writer = new Xlsx($spreadsheet);

    header('Content-Type: application/vnd.openxmlformats-officedocument.spreadsheetml.sheet');
    header('Content-Disposition: attachment; filename="file.xlsx"');
    //$writer->save('file.xlsx');

    $writer->save("php://output");
    //p(get_class_methods($writer),1);
    //return drupal_var_export($nodes);
  }

  /**
   * @return mixed
   */
  public function getResult() {
    $view = $this->viewExecute();

    //    foreach ($view->field as $field){
    //      p($field,1);
    //    }

    //    $sss = $view->render_field('field_image', 3);

    //    p(array_keys($view->field));
    //    p($sss,1);
    //    p($view);

    return $view->result;
  }

  /**
   * @return \view
   */
  public function viewExecute() {
    $view = views_get_view('auto_pars');
    $view->set_display('views_data_export_1');
    //    $view->set_arguments(array($tid));

    // change the amount of items to show
    $view->set_items_per_page(5);
    $view->pre_execute();
    $view->execute();

    return $view;
  }

  public function setCellValue($worksheet, $value) {

    $cell = $this->spreadsheet->getActiveSheet()->getCell($worksheet);
    $cell->setValue($value);
  }


  public function setImage($worksheet, $value) {

    $drawing = new \PhpOffice\PhpSpreadsheet\Worksheet\HeaderFooterDrawing();

    $drawing->setName('Paid');
    $drawing->setDescription('Paid');
    $drawing->setPath($value);
    $drawing->setHeight(36);
    $drawing->setCoordinates($worksheet);
    $drawing->setWorksheet($this->spreadsheet->getActiveSheet());
  }

  public function toExel() {

    $filePath  = "file.xlsx";

    $writer = new Xlsx($this->spreadsheet);

//    header('Content-Type: application/vnd.openxmlformats-officedocument.spreadsheetml.sheet');
//    header('Content-Disposition: attachment; filename="file.xlsx"');
    //$writer->save('file.xlsx');

//    $writer->save("php://output");
    $writer->save($filePath);
//    exit;
  }

}

