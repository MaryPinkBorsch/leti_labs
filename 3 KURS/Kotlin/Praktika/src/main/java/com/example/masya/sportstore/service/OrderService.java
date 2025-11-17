package com.example.masya.sportstore.service;

import com.example.masya.sportstore.entity.Order;
import com.example.masya.sportstore.entity.OrderStatus;
import com.example.masya.sportstore.entity.User;
import com.example.masya.sportstore.entity.Clothing;
import java.time.LocalDateTime;
import java.util.List;
import java.util.Optional;

public interface OrderService {
    List<Order> findAll();
    Optional<Order> findById(Long id);
    Order save(Order order);
    Order update(Order order);
    void deleteById(Long id);
    List<Order> findByUser(User user);
    List<Order> findByUserId(Long userId);
    List<Order> findByStatus(OrderStatus status);
    List<Order> findByOrderDateBetween(LocalDateTime start, LocalDateTime end);
    List<Order> findRecentOrders(int days);
    Order createOrder(Order order, List<Long> clothingIds);
    Order updateOrderStatus(Long orderId, OrderStatus status);
    List<Order> findByUserAndStatus(User user, OrderStatus status);
    List<Order> findOrdersContainingClothing(Clothing clothing);
    Double calculateOrderTotal(Long orderId);
    List<Order> findOrdersByClothingBrand(String brandName);
}