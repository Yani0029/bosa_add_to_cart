jQuery(document).ready(function($){
  var qty = $('.bosa-product-reference_row').length;
  var part = Math.floor(qty/6);
  $('.bosa_view_more').click(function(event){
    for (var i=1; i<= part; i++) {
      var no = i + 1;
      if($('.bosa_part_'+no).css('display') == 'none'){
        $('.bosa_view_more').show();
      }
      else {
        $('.bosa_view_more').hide();
      }
      if($('.bosa_part_'+i).css('display') == 'none'){
        $('.bosa_part_'+i).show();
        break;
      }
    }
    return false;
  });

});