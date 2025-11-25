package com.example.masya.sportstore.service;

import java.util.List;
import java.util.Optional;

import com.example.masya.sportstore.entity.Category;

public interface CategoryService {
    List<Category> findAll();
    Optional<Category> findById(Long id);
    Optional<Category> findByCategoryName(String categoryName);
    Category save(Category category);
    Category update(Category category);
    void deleteById(Long id);
    boolean existsByCategoryName(String categoryName);
}