package com.example.masya.sportstore.repository;

import com.example.masya.sportstore.entity.Category;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;
import java.util.List;
import java.util.Optional;

@Repository
public interface CategoryRepository extends JpaRepository<Category, Long> {
    // Поиск по названию
    Optional<Category> findByCategoryName(String categorName);

    // Проверка существования категории
    boolean existsByCategoryName(String categorName);
}