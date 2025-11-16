package com.example.masya.sportstore.repository;

import com.example.masya.sportstore.entity.Order;
import com.example.masya.sportstore.entity.OrderStatus;
import com.example.masya.sportstore.entity.*;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;
import java.time.LocalDateTime;
import java.util.List;

@Repository
public interface OrderRepository extends JpaRepository<Order, Long> {
    // Поиск займов пользователя
    List<Order> findByUserId(Long userId);

    // Поиск активных займов пользователя
    List<Order> findByUserIdAndStatus(Long userId, OrderStatus status);

    // Поиск займов книги
    List<Order> findByBookId(Long bookId);

    // Поиск просроченных займов
    @Query("SELECT l FROM Order l WHERE l.dueDate < :currentDate AND l.status = 'ACTIVE'")
    List<Order> findOverdueOrders(@Param("currentDate") LocalDateTime currentDate);

    // Поиск активных займов
    List<Order> findByStatus(OrderStatus status);

    // Проверка активного займа книги пользователем
@Query("SELECT l FROM Order l WHERE l.book.id = :bookId AND l.user.id = :userId AND
l.status = 'ACTIVE'")
Optional<Order> findActiveOrderByBookAndUser(@Param("bookId") Long bookId, @Param("userId")
Long userId);
}