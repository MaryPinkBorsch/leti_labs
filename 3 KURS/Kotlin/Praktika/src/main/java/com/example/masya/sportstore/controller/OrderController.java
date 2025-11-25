package com.example.masya.sportstore.controller;

import com.example.masya.sportstore.dto.OrderCreateDto;
import com.example.masya.sportstore.dto.OrderDto;
import com.example.masya.sportstore.dto.OrderUpdateDto;
import com.example.masya.sportstore.entity.Order;
import com.example.masya.sportstore.entity.OrderStatus;
import com.example.masya.sportstore.mapper.EntityDtoMapper;
import com.example.masya.sportstore.service.OrderService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.stream.Collectors;

@RestController
@RequestMapping("/api/orders")
@CrossOrigin(origins = "*")
public class OrderController {

    private final OrderService orderService;
    private final EntityDtoMapper mapper;

    @Autowired
    public OrderController(OrderService orderService, EntityDtoMapper mapper) {
        this.orderService = orderService;
        this.mapper = mapper;
    }

    @GetMapping
    public ResponseEntity<List<OrderDto>> getAllOrders() {
        List<OrderDto> orders = orderService.findAll().stream()
                .map(mapper::toOrderDto)
                .collect(Collectors.toList());
        return ResponseEntity.ok(orders);
    }

    @GetMapping("/{id}")
    public ResponseEntity<OrderDto> getOrderById(@PathVariable Long id) {
        return orderService.findById(id)
                .map(mapper::toOrderDto)
                .map(ResponseEntity::ok)
                .orElse(ResponseEntity.notFound().build());
    }

    @PostMapping
    public ResponseEntity<OrderDto> createOrder(@RequestBody OrderCreateDto orderDto) {
        Order order = new Order();
        order.setStatus(orderDto.getStatus() != null ? orderDto.getStatus() : OrderStatus.ACTIVE);
        
        Order savedOrder = orderService.createOrder(order, orderDto.getClothingIds());
        return ResponseEntity.ok(mapper.toOrderDto(savedOrder));
    }

    @PutMapping("/{id}/status")
    public ResponseEntity<OrderDto> updateOrderStatus(@PathVariable Long id, @RequestBody OrderUpdateDto updateDto) {
        Order updatedOrder = orderService.updateOrderStatus(id, updateDto.getStatus());
        return ResponseEntity.ok(mapper.toOrderDto(updatedOrder));
    }

    @DeleteMapping("/{id}")
    public ResponseEntity<Void> deleteOrder(@PathVariable Long id) {
        orderService.deleteById(id);
        return ResponseEntity.ok().build();
    }

    @GetMapping("/user/{userId}")
    public ResponseEntity<List<OrderDto>> getOrdersByUser(@PathVariable Long userId) {
        List<OrderDto> orders = orderService.findByUserId(userId).stream()
                .map(mapper::toOrderDto)
                .collect(Collectors.toList());
        return ResponseEntity.ok(orders);
    }

    @GetMapping("/status/{status}")
    public ResponseEntity<List<OrderDto>> getOrdersByStatus(@PathVariable OrderStatus status) {
        List<OrderDto> orders = orderService.findByStatus(status).stream()
                .map(mapper::toOrderDto)
                .collect(Collectors.toList());
        return ResponseEntity.ok(orders);
    }

    @GetMapping("/{id}/total")
    public ResponseEntity<Double> getOrderTotal(@PathVariable Long id) {
        Double total = orderService.calculateOrderTotal(id);
        return ResponseEntity.ok(total);
    }
}