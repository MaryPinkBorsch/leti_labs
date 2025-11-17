package com.example.masya.sportstore.entity;

import jakarta.persistence.*;

@Entity
@Table(name = "clothing")
public class Clothing {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(nullable = false)
    private String modelName;

    @Column(name = "season")
    private Integer season;

    @Column(name = "price")
    private Integer price;

    @ManyToOne
    @JoinColumn(name = "brand_id")
    private Brand brand;

    @ManyToOne
    @JoinColumn(name = "category_id")
    private Category category;
}