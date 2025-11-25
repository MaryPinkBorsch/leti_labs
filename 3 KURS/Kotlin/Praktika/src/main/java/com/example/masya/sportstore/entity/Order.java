package com.example.masya.sportstore.entity;

import java.time.LocalDateTime;
import java.util.List;

import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.EnumType;
import jakarta.persistence.Enumerated;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.JoinColumn;
import jakarta.persistence.JoinTable;
import jakarta.persistence.ManyToMany;
import jakarta.persistence.ManyToOne;
import jakarta.persistence.Table;


@Entity
@Table(name = "orders")
public class Order {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    @ManyToOne
    @JoinColumn(name = "clothing_id", nullable = false)
    private Clothing clothing;
    @ManyToOne
    @JoinColumn(name = "user_id", nullable = false)
    private User user;
    @Column(name = "Order_date", nullable = false)
    private LocalDateTime OrderDate;

    @ManyToMany
    @JoinTable(
        name = "order_clothing",
        joinColumns = @JoinColumn(name = "order_id"),
        inverseJoinColumns = @JoinColumn(name = "clothing_id")
    )
    private List<Clothing> orderedClothes;

    @Enumerated(EnumType.STRING)
    private OrderStatus status;
    // Конструкторы, геттеры, сеттеры

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public Clothing getClothing() {
        return clothing;
    }

    public void setClothing(Clothing clothing) {
        this.clothing = clothing;
    }

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public LocalDateTime getOrderDate() {
        return OrderDate;
    }

    public void setOrderDate(LocalDateTime OrderDate) {
        this.OrderDate = OrderDate;
    }

    public List<Clothing> getOrderedClothes() {
        return orderedClothes;
    }

    public void setOrderedClothes(List<Clothing> orderedClothes) {
        this.orderedClothes = orderedClothes;
    }

    public OrderStatus getStatus() {
        return status;
    }

    public void setStatus(OrderStatus status) {
        this.status = status;
    }
}
