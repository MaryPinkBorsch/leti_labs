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

class ClothingCreateDto {
    private String modelName;
    private Integer season;
    private Integer price;
    private Long brandId;
    private Long categoryId;

    // Getters and Setters
    public String getModelName() { return modelName; }
    public void setModelName(String modelName) { this.modelName = modelName; }
    public Integer getSeason() { return season; }
    public void setSeason(Integer season) { this.season = season; }
    public Integer getPrice() { return price; }
    public void setPrice(Integer price) { this.price = price; }
    public Long getBrandId() { return brandId; }
    public void setBrandId(Long brandId) { this.brandId = brandId; }
    public Long getCategoryId() { return categoryId; }
    public void setCategoryId(Long categoryId) { this.categoryId = categoryId; }
}