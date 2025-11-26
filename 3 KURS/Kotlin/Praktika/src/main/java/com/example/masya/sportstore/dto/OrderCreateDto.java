package com.example.masya.sportstore.dto;

import java.util.List;

import com.example.masya.sportstore.entity.OrderStatus;

public class OrderCreateDto {

    private Long userId;
    private List<Long> clothingIds;
    private OrderStatus status;

    // Getters and Setters
    public Long getUserId() {
        return userId;
    }

    public void setUserId(Long userId) {
        this.userId = userId;
    }

    public List<Long> getClothingIds() {
        return clothingIds;
    }

    public void setClothingIds(List<Long> clothingIds) {
        this.clothingIds = clothingIds;
    }

    public OrderStatus getStatus() {
        return status;
    }

    public void setStatus(OrderStatus status) {
        this.status = status;
    }
}
