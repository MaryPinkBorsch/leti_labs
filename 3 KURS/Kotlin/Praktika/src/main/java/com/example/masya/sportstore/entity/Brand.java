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
    private String name;

    @Column(name = "since")
    private Integer since;

    @Column(name = "rating")
    private Integer rating;

    @OneToMany(mappedBy = "brand", cascade = CascadeType.ALL)
    private List<Clothing> clothes;    

}
