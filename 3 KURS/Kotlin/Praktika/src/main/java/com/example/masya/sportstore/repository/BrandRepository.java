package com.example.masya.sportstore.repository;

import java.util.List;
import java.util.Optional;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import com.example.masya.sportstore.entity.Brand;

@Repository
public interface BrandRepository extends JpaRepository<Brand, Long> {
     
    // Поиск по Name
    List<Brand> findByNameContainingIgnoreCase(String name);

    // Поиск brands с clothings
    @Query("SELECT DISTINCT a FROM Brand a JOIN a.clothes b")
    List<Brand> findBrandsWithClothes();

    // Подсчет количества clothes brenda
    @Query("SELECT COUNT(b) FROM Clothing b WHERE b.brand.id = :brandId")
    Long countClothesByBrandId(@Param("brandId") Long brandId);

    public List<Brand> findByRatingGreaterThanEqual(Integer minRating);

    public List<Brand> findBySinceGreaterThanEqual(Integer year);

    @Query("SELECT DISTINCT a FROM Brand a WHERE a.rating > :limit")
    public List<Brand> findTopRatedBrands(@Param("limit") Integer limit);

    public boolean existsByName(String name);

    public Optional<Brand> findByName(String name);

  
}