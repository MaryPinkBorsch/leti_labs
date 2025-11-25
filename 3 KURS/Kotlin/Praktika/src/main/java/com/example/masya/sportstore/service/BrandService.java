package com.example.masya.sportstore.service;

import java.util.List;
import java.util.Optional;

import com.example.masya.sportstore.entity.Brand;

public interface BrandService {
    List<Brand> findAll();
    Optional<Brand> findById(Long id);
    Optional<Brand> findByName(String name);
    Brand save(Brand brand);
    Brand update(Brand brand);
    void deleteById(Long id);
    List<Brand> findByRatingGreaterThanEqual(Integer minRating);
    List<Brand> findBrandsEstablishedAfter(Integer year);
    List<Brand> findTopRatedBrands(int limit);
    boolean existsByName(String name);
}