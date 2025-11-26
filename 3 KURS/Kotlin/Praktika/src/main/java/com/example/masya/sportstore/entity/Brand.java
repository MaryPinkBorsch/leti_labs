package com.example.masya.sportstore.entity;

import java.util.List;

import jakarta.persistence.CascadeType;
import jakarta.persistence.Column;
import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import jakarta.persistence.OneToMany;
import jakarta.persistence.Table;

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

    public Brand(String name, Integer since, Integer rating) {
        this.name = name;
        this.rating = rating;
        this.since = since;
    }

    public Brand() {
    }

    // Геттеры и сеттеры для всех полей
    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public Integer getSince() {
        return since;
    }

    public Integer getRating() {
        return rating;
    }

    public List<Clothing> getClothes() {
        return clothes;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setSince(Integer since) {
        this.since = since;
    }

    public void setRating(Integer rating) {
        this.rating = rating;
    }

    public void setClothes(List<Clothing> clothes) {
        this.clothes = clothes;
    }

    

}
