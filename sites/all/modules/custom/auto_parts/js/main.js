(function($) {
  //jQuery code. Will proceed with a gif related example.
  $('#anigif').click(function () {
    $('#anigif2').slideToggle();
  });
  //End of gif related example. Put your code between these comments;
})(jQuery);

(function($) {

  Drupal.behaviors.export_yml = {
    attach: function (context, settings) {
      var page = 1;
      var pb;
      $('#ajax-example-progressbar-form').on('submit', function (e) {
        e.preventDefault();
        if (!pb) {
          pb = new Drupal.progressBar('progress-statussss');
          $('#progress-status').append(pb.element);
        }

        exportToYaml(page);
        return false;
      });

      function exportToYaml(page) {
        console.log('exportToYaml');
        console.log(page);
        $.post('/examples/ajax_example/progressbar/progress', {page: page}, function (data){
          if (data.status == 'OK') {
            if (!pb) {
              pb = new Drupal.progressBar('progress-statussss');
              $('#progress-status').append(pb.element);
            }

            pb.setProgress(data.percentage, data.message);
            // percentage

            if (data.percentage >= 100) {
              pb.stopMonitoring();
            } else {
              page++;
              exportToYaml(page)
            }
          }
        });
      }


      // Code to be run on page load, and
      // on ajax load added here
    }
  };


  // $( document ).ready(function() {
  //   console.log($('#page-title'));
  //   console.log(11111);
  //   $('#page-title').on('click', function (e) {
  //
  //     console.log(123123);
  //   });
  // });


  var beforeSend = Drupal.ajax.prototype.beforeSend;
  var success = Drupal.ajax.prototype.success;
  var error = Drupal.ajax.prototype.error;


  /**
   * Prepare the Ajax request before it is sent.
   */
  Drupal.ajax.prototype.beforeSend = function(xmlhttprequest, options) {
    // Вызываем код, который описан в Drupal.ajax.prototype.beforeSend в файле misc/ajax.js,
    // чтобы не нарушить работу AJAX.
    beforeSend.call(this, xmlhttprequest, options);
    if (this.progress.type == "example_progress" && $(this.element).attr("type") == "submit") {
      // Сохраняем оригинальное название кнопки, это необходимо для того, чтобы после завершения работы AJAX,
      // изменять название кнопки назад.
      this.progress.submitValue = $(this.element).attr("value");

      // Присваиваем новое название кнопки.
      $(this.element).attr("value", Drupal.t("Loading..."));
    }
  };

  /**
   * Handler for the form redirection completion.
   */
  Drupal.ajax.prototype.success = function(xmlhttprequest, options) {
    success.call(this, xmlhttprequest, options);
    // Востанавливаем значение кнопки на оригинальное.
    if (this.progress.submitValue) {
      $(this.element).attr("value", this.progress.submitValue);
    }
  };

  /**
   * Handler for the form redirection error.
   */
  Drupal.ajax.prototype.error = function (response, uri) {
    error.call(this, xmlhttprequest, options);
    if (this.progress.submitValue) {
      $(this.element).attr("value", this.progress.submitValue);
    }
  };
})(jQuery);