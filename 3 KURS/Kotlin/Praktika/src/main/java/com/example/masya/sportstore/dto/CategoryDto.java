package com.example.masya.sportstore.dto;

import java.util.List;

public class CategoryDto extends BaseDto {
    private String categoryName;
    private List<ClothingDto> clothes;

    // Constructors
    public CategoryDto() {}

    public CategoryDto(String categoryName) {
        this.categoryName = categoryName;
    }

    // Getters and Setters
    public String getCategoryName() { return categoryName; }
    public void setCategoryName(String categoryName) { this.categoryName = categoryName; }
    public List<ClothingDto> getClothes() { return clothes; }
    public void setClothes(List<ClothingDto> clothes) { this.clothes = clothes; }
}

class CategoryCreateDto {
    private String categoryName;

    // Getters and Setters
    public String getCategoryName() { return categoryName; }
    public void setCategoryName(String categoryName) { this.categoryName = categoryName; }
}