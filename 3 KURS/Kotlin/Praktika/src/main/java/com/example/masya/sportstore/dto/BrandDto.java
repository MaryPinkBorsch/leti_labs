package com.example.masya.sportstore.dto;

import java.util.List;

public class BrandDto extends BaseDto {
    private String name;
    private Integer since;
    private Integer rating;
    private List<ClothingDto> clothes;

    // Constructors
    public BrandDto() {}

    public BrandDto(String name, Integer since, Integer rating) {
        this.name = name;
        this.since = since;
        this.rating = rating;
    }

    // Getters and Setters
    public String getName() { return name; }
    public void setName(String name) { this.name = name; }
    public Integer getSince() { return since; }
    public void setSince(Integer since) { this.since = since; }
    public Integer getRating() { return rating; }
    public void setRating(Integer rating) { this.rating = rating; }
    public List<ClothingDto> getClothes() { return clothes; }
    public void setClothes(List<ClothingDto> clothes) { this.clothes = clothes; }
}

// For creating new brands (without ID)
class BrandCreateDto {
    private String name;
    private Integer since;
    private Integer rating;

    // Getters and Setters
    public String getName() { return name; }
    public void setName(String name) { this.name = name; }
    public Integer getSince() { return since; }
    public void setSince(Integer since) { this.since = since; }
    public Integer getRating() { return rating; }
    public void setRating(Integer rating) { this.rating = rating; }
}