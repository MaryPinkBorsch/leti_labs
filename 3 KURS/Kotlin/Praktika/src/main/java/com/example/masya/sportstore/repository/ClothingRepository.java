package com.example.masya.sportstore.repository;

import com.example.masya.sportstore.entity.Clothing;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;
import java.util.List;
import java.util.Optional;

@Repository
public interface ClothingRepository extends JpaRepository<Clothing, Long> {
    // Поиск по названию (частичное совпадение)
    List<Clothing> findByModelNameContainingIgnoreCase(String modelName);

    // Поиск по BrandName
    List<Clothing> findByBrandName(String BrandName);

    // All clothes whose brand has a given 'since' year
    List<Clothing> findByBrandSince(Integer since);

    // All clothes whose brand has a certain rating
    List<Clothing> findByBrandRating(Integer rating);

    // All clothes where brand.since >= some value
    List<Clothing> findByBrandSinceGreaterThanEqual(Integer since);

    // All clothes where brand.rating >= some value
    List<Clothing> findByBrandRatingGreaterThanEqual(Integer rating);

    // Combined filter: brand.since >= X AND brand.rating >= Y
    List<Clothing> findByBrandSinceGreaterThanEqualAndBrandRatingGreaterThanEqual(Integer since, Integer rating);

    // Поиск по категории
    List<Clothing> findByCategoryId(Long categoryId);

}