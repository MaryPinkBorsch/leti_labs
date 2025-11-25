package com.example.masya.sportstore.repository;

import java.util.List;
import java.util.Optional;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import com.example.masya.sportstore.entity.Category;

@Repository
public interface CategoryRepository extends JpaRepository<Category, Long> {
    // Поиск по названию
    Optional<Category> findByCategoryName(String categorName);

    // Проверка существования категории
    boolean existsByCategoryName(String categorName);

    public List<Category> findCategoriesWithClothing();

    public List<Category> findPopularCategories(int limit);
}