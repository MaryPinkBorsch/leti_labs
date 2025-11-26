package com.example.masya.sportstore.dto;

import com.example.masya.sportstore.entity.OrderStatus;

public class OrderUpdateDto {

    private OrderStatus status;

    // Getters and Setters
    public OrderStatus getStatus() {
        return status;
    }

    public void setStatus(OrderStatus status) {
        this.status = status;
    }
}
