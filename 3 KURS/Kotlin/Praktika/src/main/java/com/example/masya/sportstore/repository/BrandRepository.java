package com.example.masya.sportstore.repository;

import com.example.masya.sportstore.entity.Brand;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.stereotype.Repository;
import java.util.List;
import java.util.Optional;

@Repository
public interface BrandRepository extends JpaRepository<Brand, Long> {
     
    // Поиск по Name
    List<Brand> findByNameContainingIgnoreCase(String Name);

    // Поиск brands с clothings
    @Query("SELECT DISTINCT a FROM brand a JOIN a.clothes b")
    List<Brand> findBrandsWithClothes();

    // Подсчет количества clothes brenda
    @Query("SELECT COUNT(b) FROM clothing b WHERE b.brand_id = :BrandId")
    Long countClothesByBrandId(Long BrandId);
}