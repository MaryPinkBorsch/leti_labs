package com.example.masya.sportstore.dto;

// For creating new brands (without ID)
public class BrandCreateDto {
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