package com.example.masya.sportstore.service;

import com.example.masya.sportstore.entity.Clothing;

import java.util.List;
import java.util.Optional;

public interface StoreService {
    List<Clothing> findAllClothings();

    Optional<Clothing> findClothingById(Long id);
    // TODO: нужны методы для работы с заказами

    List<Clothing> findClothingsByBrand(String brandName);

    List<Clothing> findClothingsByCategory(String categoryName);

}
