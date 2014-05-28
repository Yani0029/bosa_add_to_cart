jQuery(document).ready(function($){
  $('.bosa_view_more').click(function(event){
    var id = $(this).attr('id');
    var qty = $('.bosa-product-reference_row').length;
    var part = Math.floor(qty/6);
    jQuery.get("bosa_add_to_cart/ajax/view/"+id+"/"+part, function(data){
      $('.js-bose-view-more').html(data);
    });
  });
});