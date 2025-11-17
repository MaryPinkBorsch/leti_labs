package com.example.masya.sportstore.repository;

import com.example.masya.sportstore.entity.Order;
import com.example.masya.sportstore.entity.OrderStatus;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;
import java.time.LocalDateTime;
import java.util.List;
import java.util.Optional;

@Repository
public interface OrderRepository extends JpaRepository<Order, Long> {
    // Поиск займов пользователя
    List<Order> findByUserId(Long userId);

    // Поиск активных займов пользователя
    List<Order> findByUserIdAndStatus(Long userId, OrderStatus status);

    // Поиск активных займов
    List<Order> findByStatus(OrderStatus status);
}