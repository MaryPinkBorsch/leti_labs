package com.example.masya.sportstore.service.impl;

import java.util.List;
import java.util.Optional;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.example.masya.sportstore.entity.Category;
import com.example.masya.sportstore.repository.CategoryRepository;
import com.example.masya.sportstore.service.CategoryService;

@Service
@Transactional
public class CategoryServiceImpl implements CategoryService {

    private final CategoryRepository categoryRepository;

    @Autowired
    public CategoryServiceImpl(CategoryRepository categoryRepository) {
        this.categoryRepository = categoryRepository;
    }

    @Override
    @Transactional(readOnly = true)
    public List<Category> findAll() {
        return categoryRepository.findAll();
    }

    @Override
    @Transactional(readOnly = true)
    public Optional<Category> findById(Long id) {
        return categoryRepository.findById(id);
    }

    @Override
    @Transactional(readOnly = true)
    public Optional<Category> findByCategoryName(String categoryName) {
        return categoryRepository.findByCategoryName(categoryName);
    }

    @Override
    public Category save(Category category) {
        return categoryRepository.save(category);
    }

    @Override
    public Category update(Category category) {
        if (!categoryRepository.existsById(category.getId())) {
            throw new RuntimeException("Category not found with id: " + category.getId());
        }
        return categoryRepository.save(category);
    }

    @Override
    public void deleteById(Long id) {
        categoryRepository.deleteById(id);
    }

    @Override
    @Transactional(readOnly = true)
    public boolean existsByCategoryName(String categoryName) {
        return categoryRepository.existsByCategoryName(categoryName);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Category> findCategoriesWithClothing() {
        return categoryRepository.findCategoriesWithClothing();
    }

    @Override
    @Transactional(readOnly = true)
    public List<Category> findPopularCategories(int limit) {
        return categoryRepository.findPopularCategories(limit);
    }
}
