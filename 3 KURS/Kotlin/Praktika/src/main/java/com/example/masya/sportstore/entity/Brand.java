package com.example.masya.sportstore.entity;

import jakarta.persistence.*;
import java.util.List;

@Entity
@Table(name = "brand")
public class Brand {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(nullable = false, unique = true)
    private String brand_name;

    @Column(name = "since")
    private Integer since;

    @Column(name = "stars")
    private Integer rating;

    @OneToMany(mappedBy = "brand", cascade = CascadeType.ALL)
    private List<Clothing> clothes;    

}
