package com.example.masya.sportstore.service;

import com.example.masya.sportstore.entity.Clothing;
import com.example.masya.sportstore.entity.Brand;
import com.example.masya.sportstore.entity.Category;
import java.util.List;
import java.util.Optional;

public interface ClothingService {
    List<Clothing> findAll();
    Optional<Clothing> findById(Long id);
    Clothing save(Clothing clothing);
    Clothing update(Clothing clothing);
    void deleteById(Long id);
    List<Clothing> findByBrand(Brand brand);
    List<Clothing> findByCategory(Category category);
    List<Clothing> findByBrandAndCategory(Brand brand, Category category);
    List<Clothing> findByPriceBetween(Integer minPrice, Integer maxPrice);
    List<Clothing> findByModelNameContaining(String modelName);
    List<Clothing> findBySeason(Integer season);
    List<Clothing> findTopRatedByBrand(Long brandId, int limit);
    List<Clothing> findByCategoryName(String categoryName);
    List<Clothing> findByBrandName(String brandName);
}