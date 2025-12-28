package com.example.masya.sportstore.controller;

import com.example.masya.sportstore.SportstoreApplication;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.http.MediaType;
import org.springframework.test.web.servlet.MockMvc;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;
import org.springframework.transaction.annotation.Transactional;

@SpringBootTest(classes = SportstoreApplication.class)
@AutoConfigureMockMvc // ← Исправьте: AutoConfigureMockMvc вместо AutoConfigureWebMvc
@Transactional
class DataLoaderControllerTest {

    @Autowired
    private MockMvc mockMvc;    

    @Test
    void testLoadAllDataWithAdminRole() throws Exception {
        mockMvc.perform(post("/api/test-data/load-all")
                .contentType(MediaType.APPLICATION_JSON))
                .andExpect(status().isOk());
    }
}