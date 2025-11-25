package com.example.masya.sportstore.repository;

import java.time.LocalDateTime;
import java.util.List;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import com.example.masya.sportstore.entity.Order;
import com.example.masya.sportstore.entity.OrderStatus;
import com.example.masya.sportstore.entity.User;

@Repository
public interface OrderRepository extends JpaRepository<Order, Long> {
    // Поиск займов пользователя
    List<Order> findByUserId(Long userId);

    // Поиск активных займов пользователя
    List<Order> findByUserIdAndStatus(Long userId, OrderStatus status);

    // Поиск активных займов
    List<Order> findByStatus(OrderStatus status);

    public List<Order> findByUser(User user);


    public List<Order> findByOrderDateAfter(LocalDateTime startDate);

    public List<Order> findByUserAndStatus(User user, OrderStatus status);


}