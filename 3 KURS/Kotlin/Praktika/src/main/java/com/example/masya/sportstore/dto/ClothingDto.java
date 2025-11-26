package com.example.masya.sportstore.dto;

public class ClothingDto extends BaseDto {
    private String modelName;
    private Integer season;
    private Integer price;
    private BrandDto brand;
    private CategoryDto category;

    // Constructors
    public ClothingDto() {}

    public ClothingDto(String modelName, Integer season, Integer price) {
        this.modelName = modelName;
        this.season = season;
        this.price = price;
    }

    // Getters and Setters
    public String getModelName() { return modelName; }
    public void setModelName(String modelName) { this.modelName = modelName; }
    public Integer getSeason() { return season; }
    public void setSeason(Integer season) { this.season = season; }
    public Integer getPrice() { return price; }
    public void setPrice(Integer price) { this.price = price; }
    public BrandDto getBrand() { return brand; }
    public void setBrand(BrandDto brand) { this.brand = brand; }
    public CategoryDto getCategory() { return category; }
    public void setCategory(CategoryDto category) { this.category = category; }
}
 