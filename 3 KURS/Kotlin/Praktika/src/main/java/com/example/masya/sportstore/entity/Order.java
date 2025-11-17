package com.example.masya.sportstore.entity;

import jakarta.persistence.*;
import java.time.LocalDateTime;
import java.util.List;


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
}
