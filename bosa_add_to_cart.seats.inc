<?php
/**
 * bosa_add_to_cart.seats.inc
 **/

/**
 * @param $product_id
 * @param $delta : the place no of the date field. like : ['und'][$delta]['value']
 */
function bosa_reserved_seats_available($product_id,$delta = 0){
  $product = commerce_product_load($product_id);

  // The new product type "BOSA product" that contains product date and stock
  if(isset($product->field_product_date_and_stock)) {
    $info = field_get_items('commerce_product', $product, 'field_product_date_and_stock');
    $query = db_query('SELECT SUM(quantity) AS quantity FROM `commerce_product` p, `commerce_line_item` l,`commerce_order` o,
                      `field_data_field_bosa_product_delta`
                       WHERE sku = line_item_label
                       AND o.`status` <> :canceled
                       AND o.order_id = l.order_id
                       AND `product_id` = :product_id
                       AND`line_item_id` = `entity_id`
                       AND `field_bosa_product_delta_value` = :delta',
                    array(':canceled' => 'canceled',':product_id' => $product_id, ':delta' => $delta));

    $result = $query->fetch();
    if($result){
      $count = reset($result);
    }
    if ($info[$delta]['product_stock'] == 0) {
      $seats = "all";
    }
    else {
      $seats = (int)($info[$delta]['product_stock'] - $count);
    }
    return $seats;

    break;
  }

  // Other product type with commerce_stock
  if($seats_field = field_get_items('commerce_product', $product, 'commerce_stock')){
    $query = db_query('SELECT SUM(quantity) as quantity FROM `commerce_product` p, `commerce_line_item`l,`commerce_order` o
                       WHERE p.sku = l.line_item_label AND o.`status` <> :canceled
                       AND o.order_id = l.order_id
                       AND `product_id`= :product_id',
                      array(':canceled' => 'canceled',':product_id' => $product_id));

    $result = $query->fetch();
    if($result){
      $count = reset($result);
    }
    $seats = $seats_field[0]['value'];
    $seats_available = $seats - $count;
    return (int)$seats_available;
  }
  // Product type that does not contain a stock field.
  else {
    return 'all';
  }
}
/*
 * used to get bosa_produkt orders.
 */
function bosa_reserved_bosa_product_order_info($product_id,$delta = NULL){
  $product = commerce_product_load($product_id);
  $cancel_orders = array();
  // The new product type "BOSA product" that contains multiple product-date and stock
  if($product->type == 'bosa_produkt') {
    $query = db_query('SELECT order_id, line_item_id, quantity
                        FROM `commerce_product`, `commerce_line_item`,
                        `field_data_field_bosa_product_delta`
                        WHERE sku = line_item_label
                        AND `product_id` = :product_id
                        AND `line_item_id` = `entity_id`
                        AND `field_bosa_product_delta_value` = :delta',
                    array(':product_id' => $product->product_id, ':delta' => $delta));
    $result = $query->fetchAll();

    if($result){
      foreach($result as $row) {
        // Load order and check for status.
        $order = commerce_order_load($row->order_id);
        if($order->status == 'completed'){
          $cancel_orders[] = array(
            'product_id' => $product->product_id,
            'order_id' => $row->order_id,
            'line_item_id' => $row->line_item_id,
            'quantity' => $row->quantity,
            'delta' => $delta,
          );
        }
      }
    }
  }
  return $cancel_orders;
}

function bosa_reserved_delta_check_by_line_item($line_item_id) {
  $info = array();
  $query = db_query('SELECT `line_item_id`,`field_bosa_product_delta_value`
                     FROM `commerce_line_item`, `field_data_field_bosa_product_delta`
                     WHERE `line_item_id` = `entity_id`
                     AND `line_item_id` = :line_item_id',
                    array(':line_item_id' => $line_item_id));
  $result = $query->fetchAll();
  if ($result) {
    foreach($result as $row) {
      $info[$row->line_item_id] = $row->field_bosa_product_delta_value;
    }
  }
  return $info;
}

function bosa_reserved_get_date_by_line_item($line_item_id) {
  $query = db_query('SELECT `field_bosa_selected_date_value`
                     FROM `commerce_line_item`, `field_data_field_bosa_selected_date`
                     WHERE `line_item_id` = `entity_id`
                     AND `line_item_id` = :line_item_id',
                    array(':line_item_id' => $line_item_id));
  $result = $query->fetch();
  if ($result) {
    $date = reset($result);
  }
  else {
    $date = "";
  }
  return $date;
}