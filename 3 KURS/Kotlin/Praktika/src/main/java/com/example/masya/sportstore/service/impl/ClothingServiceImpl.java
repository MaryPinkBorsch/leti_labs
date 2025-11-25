package com.example.masya.sportstore.service.impl;

import java.util.List;
import java.util.Optional;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.example.masya.sportstore.entity.Brand;
import com.example.masya.sportstore.entity.Category;
import com.example.masya.sportstore.entity.Clothing;
import com.example.masya.sportstore.repository.ClothingRepository;
import com.example.masya.sportstore.service.ClothingService;

@Service
@Transactional
public class ClothingServiceImpl implements ClothingService {

    private final ClothingRepository clothingRepository;

    @Autowired
    public ClothingServiceImpl(ClothingRepository clothingRepository) {
        this.clothingRepository = clothingRepository;
    }

    @Override
    @Transactional(readOnly = true)
    public List<Clothing> findAll() {
        return clothingRepository.findAll();
    }

    @Override
    @Transactional(readOnly = true)
    public Optional<Clothing> findById(Long id) {
        return clothingRepository.findById(id);
    }

    @Override
    public Clothing save(Clothing clothing) {
        return clothingRepository.save(clothing);
    }

    @Override
    public Clothing update(Clothing clothing) {
        if (!clothingRepository.existsById(clothing.getId())) {
            throw new RuntimeException("Clothing item not found with id: " + clothing.getId());
        }
        return clothingRepository.save(clothing);
    }

    @Override
    public void deleteById(Long id) {
        clothingRepository.deleteById(id);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Clothing> findByBrand(Brand brand) {
        return clothingRepository.findByBrand(brand);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Clothing> findByCategory(Category category) {
        return clothingRepository.findByCategory(category);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Clothing> findByBrandAndCategory(Brand brand, Category category) {
        return clothingRepository.findByBrandAndCategory(brand, category);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Clothing> findByPriceBetween(Integer minPrice, Integer maxPrice) {
        return clothingRepository.findByPriceBetween(minPrice, maxPrice);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Clothing> findByModelNameContaining(String modelName) {
        return clothingRepository.findByModelNameContainingIgnoreCase(modelName);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Clothing> findBySeason(Integer season) {
        return clothingRepository.findBySeason(season);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Clothing> findTopRatedByBrand(Long brandId, int limit) {
        return clothingRepository.findTopRatedByBrandId(brandId, limit);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Clothing> findByCategoryName(String categoryName) {
        return clothingRepository.findByCategoryCategoryName(categoryName);
    }

    @Override
    @Transactional(readOnly = true)
    public List<Clothing> findByBrandName(String brandName) {
        return clothingRepository.findByBrandName(brandName);
    }
}
