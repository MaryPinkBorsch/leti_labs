package com.example.masya.sportstore.dto;

import com.example.masya.sportstore.entity.OrderStatus;
import java.time.LocalDateTime;
import java.util.List;

public class OrderDto extends BaseDto {
    private LocalDateTime orderDate;
    private OrderStatus status;
    private UserDto user;
    private List<ClothingDto> orderedClothes;
    private Double totalAmount;

    // Constructors
    public OrderDto() {}

    // Getters and Setters
    public LocalDateTime getOrderDate() { return orderDate; }
    public void setOrderDate(LocalDateTime orderDate) { this.orderDate = orderDate; }
    public OrderStatus getStatus() { return status; }
    public void setStatus(OrderStatus status) { this.status = status; }
    public UserDto getUser() { return user; }
    public void setUser(UserDto user) { this.user = user; }
    public List<ClothingDto> getOrderedClothes() { return orderedClothes; }
    public void setOrderedClothes(List<ClothingDto> orderedClothes) { this.orderedClothes = orderedClothes; }
    public Double getTotalAmount() { return totalAmount; }
    public void setTotalAmount(Double totalAmount) { this.totalAmount = totalAmount; }
}

class OrderCreateDto {
    private Long userId;
    private List<Long> clothingIds;
    private OrderStatus status;

    // Getters and Setters
    public Long getUserId() { return userId; }
    public void setUserId(Long userId) { this.userId = userId; }
    public List<Long> getClothingIds() { return clothingIds; }
    public void setClothingIds(List<Long> clothingIds) { this.clothingIds = clothingIds; }
    public OrderStatus getStatus() { return status; }
    public void setStatus(OrderStatus status) { this.status = status; }
}

class OrderUpdateDto {
    private OrderStatus status;

    // Getters and Setters
    public OrderStatus getStatus() { return status; }
    public void setStatus(OrderStatus status) { this.status = status; }
}