package com.example.masya.sportstore.service.impl;

import com.example.masya.sportstore.entity.*;
import com.example.masya.sportstore.repository.ClothingRepository;
import com.example.masya.sportstore.repository.OrderRepository;
import com.example.masya.sportstore.repository.UserRepository;
import com.example.masya.sportstore.service.OrderService;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDateTime;
import java.util.List;
import java.util.Optional;

@Service
@Transactional
public class OrderServiceImpl implements OrderService {

    private final OrderRepository orderRepository;
    private final UserRepository userRepository;
    private final ClothingRepository clothingRepository;

    @Autowired
    public OrderServiceImpl(OrderRepository orderRepository,
            UserRepository userRepository,
            ClothingRepository clothingRepository) {
        this.orderRepository = orderRepository;
        this.userRepository = userRepository;
        this.clothingRepository = clothingRepository;
    }

    @Override
    @Transactional(readOnly = true)
    public List<Order> findAll() {
        return orderRepository.findAll();
    }

    @Override
    @Transactional(readOnly = true)
    public Optional<Order> findById(Long id) {
        return orderRepository.findById(id);
    }

    @Override
    public Order save(Order order) {
        order.setOrderDate(LocalDateTime.now());
        if (order.getStatus() == null) {
            order.setStatus(OrderStatus.ACTIVE);
        }
        return orderRepository.save(order);
    }

    @Override
    public Order update(Order order) {
        if (!orderRepository.existsById(order.getId())) {
            throw new RuntimeException("Order not found with id: " + order.getId());
        }
        return orderRepository.save(order);
    }

    @Override
    public void deleteById(Long id) {
        orderRepository.deleteById(id);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Order> findByUser(User user) {
        return orderRepository.findByUser(user);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Order> findByUserId(Long userId) {
        return orderRepository.findByUserId(userId);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Order> findByStatus(OrderStatus status) {
        return orderRepository.findByStatus(status);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Order> findByOrderDateBetween(LocalDateTime start, LocalDateTime end) {
        return orderRepository.findByOrderDateBetween(start, end);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Order> findRecentOrders(int days) {
        LocalDateTime startDate = LocalDateTime.now().minusDays(days);
        return orderRepository.findByOrderDateAfter(startDate);
    }

    @Override
    public Order createOrder(Order order, List<Long> clothingIds) {
        // Verify user exists
        User user = userRepository.findById(order.getUser().getId())
                .orElseThrow(() -> new RuntimeException("User not found with id: " + order.getUser().getId()));

        // Get clothing items
        List<Clothing> clothingItems = clothingRepository.findAllById(clothingIds);
        if (clothingItems.size() != clothingIds.size()) {
            throw new RuntimeException("Some clothing items not found");
        }

        order.setUser(user);
        order.setOrderedClothes(clothingItems);
        order.setOrderDate(LocalDateTime.now());
        order.setStatus(OrderStatus.ACTIVE);

        return orderRepository.save(order);
    }

    @Override
    public Order updateOrderStatus(Long orderId, OrderStatus status) {
        Order order = orderRepository.findById(orderId)
                .orElseThrow(() -> new RuntimeException("Order not found with id: " + orderId));

        order.setStatus(status);
        return orderRepository.save(order);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Order> findByUserAndStatus(User user, OrderStatus status) {
        return orderRepository.findByUserAndStatus(user, status);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Order> findOrdersContainingClothing(Clothing clothing) {
        return orderRepository.findOrdersContainingClothing(clothing.getId());
    }

    @Override
    @Transactional(readOnly = true)
    public Double calculateOrderTotal(Long orderId) {
        Order order = orderRepository.findById(orderId)
                .orElseThrow(() -> new RuntimeException("Order not found with id: " + orderId));

        return order.getOrderedClothes().stream()
                .mapToDouble(clothing -> clothing.getPrice() != null ? clothing.getPrice().doubleValue() : 0.0)
                .sum();
    }

    @Override
    @Transactional(readOnly = true)
    public List<Order> findOrdersByClothingBrand(String brandName) {
        return orderRepository.findOrdersByClothingBrand(brandName);
    }
}
