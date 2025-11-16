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
    List<Clothing> findByModelNameContainingIgnoreCase(String model_name);

    // Поиск по BrandName
    List<Clothing> findByBrandName(String BrandName);

    // Поиск по since
    List<Clothing> findBySince(Integer since);

    // Поиск по rating
    List<Clothing> findByRating(Integer rating);

    // Поиск по категории
    List<Clothing> findByCategoryId(Long categoryId);

}