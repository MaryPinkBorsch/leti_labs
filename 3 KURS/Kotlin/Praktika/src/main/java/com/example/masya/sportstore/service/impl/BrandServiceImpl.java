package com.example.masya.sportstore.service.impl;

import com.example.masya.sportstore.entity.Brand;
import com.example.masya.sportstore.repository.BrandRepository;
import com.example.masya.sportstore.service.BrandService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;
import java.util.Optional;

@Service
@Transactional
public class BrandServiceImpl implements BrandService {

    private final BrandRepository brandRepository;

    @Autowired
    public BrandServiceImpl(BrandRepository brandRepository) {
        this.brandRepository = brandRepository;
    }

    @Override
    @Transactional(readOnly = true)
    public List<Brand> findAll() {
        return brandRepository.findAll();
    }

    @Override
    @Transactional(readOnly = true)
    public Optional<Brand> findById(Long id) {
        return brandRepository.findById(id);
    }

    @Override
    @Transactional(readOnly = true)
    public Optional<Brand> findByName(String name) {
        return brandRepository.findByName(name);
    }

    @Override
    public Brand save(Brand brand) {
        return brandRepository.save(brand);
    }

    @Override
    public Brand update(Brand brand) {
        if (!brandRepository.existsById(brand.getId())) {
            throw new RuntimeException("Brand not found with id: " + brand.getId());
        }
        return brandRepository.save(brand);
    }

    @Override
    public void deleteById(Long id) {
        brandRepository.deleteById(id);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Brand> findByRatingGreaterThanEqual(Integer minRating) {
        return brandRepository.findByRatingGreaterThanEqual(minRating);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Brand> findBrandsEstablishedAfter(Integer year) {
        return brandRepository.findBySinceGreaterThanEqual(year);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Brand> findTopRatedBrands(int limit) {
        return brandRepository.findTopRatedBrands(limit);
    }

    @Override
    @Transactional(readOnly = true)
    public boolean existsByName(String name) {
        return brandRepository.existsByName(name);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Brand> findBrandsWithClothingInCategory(String categoryName) {
        return brandRepository.findBrandsWithClothingInCategory(categoryName);
    }
}