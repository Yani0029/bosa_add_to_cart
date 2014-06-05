jQuery(document).ready(function($){
  var qty = $('.bosa-product-reference_row').length;
  var part = Math.floor(qty/6);
  if (part == 1 && qty > 6) {
    part = 2;
  }

  $('.bosa_view_later').show();
  $('.bosa_view_later').click(function(event){
    for (var i=0; i<= part; i++) {
      var next = i + 1;
      var prev = i-1;
      if($('.bosa_part_'+i).css('display') == 'block'){
        if (i !== (part-1)) {
          $('.bosa_part_'+i).hide();
        }
        $('.bosa_view_earlier').show();
        $('.bosa_part_'+next).show();

        if (i== (part -1)) {
          $('.bosa_view_later').hide();
        }
        break;
      }
    }
    return false;
  });
   $('.bosa_view_earlier').click(function(event){
    for (var i=0; i<= part; i++) {
      var next = i + 1;
      var prev = i-1;
      if($('.bosa_part_'+prev).css('display') == 'none'){
        $('.bosa_view_earlier').show();
      }
      if($('.bosa_part_'+i).css('display') == 'block'){
        if (i !== 0) {
          $('.bosa_part_'+i).hide();
        }
        if (i == 0) {
          $('.bosa_view_earlier').hide();
        }
        $('.bosa_view_later').show();
        $('.bosa_part_'+prev).show();
        break;
      }
    }
    return false;
  });
  var calculatePrice = function(){
  var allcheckboxs =  $('.bosa-product-reference input');
  var items = $('.bosa_cart_amount_all input');
  var summary = $('#edit-total-price');
  var price = 0;
  items.each(function(){
    var cnt = parseInt($(this).val());
    if(cnt>10000){
      cnt = 10000;
      $(this).val(cnt)
    }else if(cnt<0){
      cnt = 1;
      $(this).val(cnt)
    }
    var p=parseInt($(this).attr('data-price'))*cnt;

    if(!isNaN(p)) {
      price+=p;
    }
   });

  if(allcheckboxs.size()>0){
    var cnt = allcheckboxs.filter(':checked').size();
    price*=cnt;
  }
  var pris = (price/100).toFixed(2).toString().replace(".", ",");
  summary.text(pris);
  //console.log(p);
  }

  $('#bosa-add-to-cart-form input').live("keyup change",calculatePrice);
  calculatePrice();
});