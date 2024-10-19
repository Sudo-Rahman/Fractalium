//
// Created by sr-71 on 19/12/2023.
//

#ifndef FRACTALIUM_HISTORY_HPP
#define FRACTALIUM_HISTORY_HPP

#include <QImage>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <Double.hpp>
#include <Fractal.hpp>
#include <Color.hpp>

namespace Fractalium {
    struct HistoryImage {
        std::vector <std::vector<Color>> image{};

        template<typename Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & image;
        }

        HistoryImage() = default;

        HistoryImage(const QImage &img) {
            image = std::vector < std::vector < Color >> (img.height(), std::vector<Color>(img.width()));

            for (int i = 0; i < img.height(); ++i) {
                for (int j = 0; j < img.width(); ++j) {
                    QColor color = img.pixelColor(j, i);
                    image[i][j].r = color.red();
                    image[i][j].g = color.green();
                    image[i][j].b = color.blue();
                }
            }
        }

        [[nodiscard]] QImage toQImage() const {
            QImage img(image[0].size(), image.size(), QImage::Format_RGB32);

            for (int i = 0; i < image.size(); ++i) {
                for (int j = 0; j < image[0].size(); ++j) {
                    img.setPixelColor(j, i, QColor(image[i][j].r, image[i][j].g, image[i][j].b));
                }
            }

            return img;
        }

    };

    struct History {
        HistoryImage image;
        std::pair <Fractalium::Double, Fractalium::Double> offset;
        Fractalium::Double step_coord;

        template<typename Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & image;
            ar & offset;
            ar & step_coord;
        }
    };

    struct SnapshotHistory {
        std::vector <History> history_back;
        std::vector <History> history_front;
        Fractal fractal;

        SnapshotHistory() = default;

        SnapshotHistory(const std::vector <History> &history_back, const std::vector <History> &history_front,
                        const Fractal &fractal)
                : history_back(history_back), history_front(history_front), fractal(fractal) {}

        template<typename Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & history_back;
            ar & history_front;
            ar & fractal;
        }
    };


    /**
     * @brief Crée un snapshot
     * @param filename
     * @param snapshotHistory
     */
    static void makeSnapshot(const std::string &filename, const SnapshotHistory &snapshotHistory) {
        std::ofstream ofs(filename);
        boost::archive::text_oarchive oa(ofs);
        oa << snapshotHistory;
    }

    /**
     * @brief Importe un snapshot
     * @param filename
     * @param snapshotHistory
     */
    static void importSnapshot(const std::string &filename, SnapshotHistory &snapshotHistory) {
        std::ifstream ifs(filename);
        boost::archive::text_iarchive ia(ifs);
        ia >> snapshotHistory;
    }
}

#endif //FRACTALIUM_HISTORY_HPP
