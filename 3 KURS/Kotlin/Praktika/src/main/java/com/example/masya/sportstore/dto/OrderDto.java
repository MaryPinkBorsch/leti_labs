package com.example.masya.sportstore.dto;

import java.time.LocalDateTime;
import java.util.List;

import com.example.masya.sportstore.entity.OrderStatus;

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
 